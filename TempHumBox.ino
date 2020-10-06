#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <PubSubClient.h>
#include  <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "myWiFiManager.h"

#include "DHT.h"

/***************************************/
// DHT
#define DHTPIN 13
#define DHTTYPE DHT11

#define WLPIN 12

char *thingsboardServer = "192.168.110.110";
WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;

DHT dht(DHTPIN, DHTTYPE);

/***************************************/

void setup() {
  //cfg._AccessPointConnectingTimeOut=300;
  Serial.begin(115200);
  Serial.println("Initializing...");  
  myWiFiManagerSetup();
/**************************************/

  dht.begin();
  delay(10);
  pinMode(WLPIN, INPUT);
  delay(10);

  delay(10);
  thingsboardServer=cfg._Server_IP;
  Serial.println("Connecting To TB Server");
  client.setServer( thingsboardServer, cfg._PortNumber );
  lastSend = 0;
/**************************************/
}

void loop() {
  server.handleClient();
  
/*****************************************/
if(APisConnected)
{
  if(!client.connected())
  {
    connectToTB();
  }
  
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }
  client.loop();
}
else
{
  //Serial.println("Welcome,");
  //Serial.println("Waiting for first config");
}
/****************************************/  
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting sensors data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  int w=digitalRead(WLPIN);
  Serial.print("WaterLeakage: ");
  Serial.print(w);
  Serial.print(" %\w");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  String WaterLeakage = String(w);
  String temperature = String(t);
  String humidity = String(h);


  // Just debug messages
  Serial.print( "Sending WaterLeakage : [" );
  Serial.print( WaterLeakage); Serial.println( ";" );
  Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += String(cfg._Topic); payload += "_Temp:";
  payload += temperature; payload += ",";
  payload += String(cfg._Topic);  payload += "_Hum:";
  payload += humidity; 
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  if(client.publish( "v1/devices/me/telemetry", attributes ))
  {
    Serial.println("Published: ");
    Serial.println( attributes );
  }else Serial.println("Error in publishing");
}

void connectToTB() 
{
    Serial.print("Connecting to ThingsBoard node ...");
    if ( client.connect(cfg._ClientID, cfg._TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 5000 );
    }
}




