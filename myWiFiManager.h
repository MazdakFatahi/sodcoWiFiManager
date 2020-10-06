void LoadConfig();
void displayInBrowser(struct Config );
void write_config();
bool read_config();
void handleSubmit();
void handelRoot();
String getValue(String , char , int );
void covertToIP(char *[]);
void myWiFiManagerSetup();

String WebTempToDisplay="";
boolean APisConnected=false;

struct Config
{
  char _SSID[20]="";
  char _Password[64]="";
  char _Static_IP[16]="";
  char _SubnetMask[16]="";
  char _Gateway[16]="";
  char _HostName[20]="";
  char _Server_IP[16]="";
  int _PortNumber=1880;
  char _TOKEN[50]="";
  char _SoftAPName[20]="";
  char _SoftAPPassword[64]="";
  char _ClientID[20]="";
  char _Topic[20]="";
  int _AccessPointConnectingTimeOut=0;
};
struct Config cfg;
ESP8266WebServer server(83);


void LoadConfig()
{
 String response="<p><h2>Current configuration: </h2><hr>";
 response += "SSID="+String(cfg._SSID);
 response +="<br>";
 response += "Password="+String(cfg._Password);
 response +="<br>";
 response += "Subnet="+String(cfg._SubnetMask);
 response +="<br>";
 response += "Gateway="+String(cfg._Gateway);
 response +="<br>";
 response += "Node IP="+String(cfg._Static_IP);
 response +="<br>";
 response += "HostName="+String(cfg._HostName);
 response +="<br>";
 response += "Server="+String(cfg._Server_IP);
 response +="<br>";
 response += "Port="+String(cfg._PortNumber);
 response +="<br>";
 response += "AccessPointConnectingTimeOut="+String(cfg._AccessPointConnectingTimeOut);
 response +="<br>";
 response += "TOKEN="+String(cfg._TOKEN);
 response +="<br>";
 response += "SoftAPName="+String(cfg._SoftAPName);
 response +="<br>";
 response += "SoftAPPassword="+String(cfg._SoftAPPassword);
 response +="<br>";
 response += "ClientID="+String(cfg._ClientID);
 response +="<br>";
 response += "Topic="+String(cfg._Topic);
 response +="<br>";
 response +="</P><hr><BR>";
 response +="<H4><a href=\"/\">go home</a></H4><br>";

 WebTempToDisplay=response;
 
 server.send(200, "text/html", response);
}


void displayInBrowser(struct Config data)
{
 String response="<p><h2>Current configuration: </h2><hr>";
 response += "SSID="+String(data._SSID);
 response +="<br>";
 response += "Password="+String(data._Password);
 response +="<br>";
 response += "Subnet="+String(data._SubnetMask);
 response +="<br>";
 response += "Gateway="+String(data._Gateway);
 response +="<br>";
 response += "Node IP="+String(data._Static_IP);
 response +="<br>";
 response += "HostName="+String(data._HostName);
 response +="<br>";
 response += "Server="+String(data._Server_IP);
 response +="<br>";
 response += "Port="+String(data._PortNumber);
 response +="<br>";
 response += "AccessPointConnectingTimeOut="+String(data._AccessPointConnectingTimeOut);
 response +="<br>";
 response += "TOKEN="+String(data._TOKEN);
 response +="<br>";
 response += "SoftAPName="+String(data._SoftAPName);
 response +="<br>";
 response += "SoftAPPassword="+String(data._SoftAPPassword);
 response +="<br>";
 response += "ClientID="+String(data._ClientID);
 response +="<br>";
 response += "Topic="+String(data._Topic);
 response +="<br>";
 response +="</P><hr><BR>";
 response +="<H4><a href=\"/\">go home</a></H4><br>";

 WebTempToDisplay=response;
 
 server.send(200, "text/html", response);
}


void write_config()
{
    displayInBrowser(cfg); 
    
    DynamicJsonBuffer jsonBuffer;
    
    JsonObject& json = jsonBuffer.createObject();     
    
    /*strcpy( json["ssid"],cfg._SSID);
    strcpy(json["pass"],cfg._Password);
    strcpy( json["IP"],cfg._Static_IP);
    strcpy(json["Subnet"],cfg._SubnetMask);
    strcpy(json["Gateway"],cfg._Gateway);
    strcpy(json["IOT_Server"],cfg._Server_IP);*/
    json["ssid"]=cfg._SSID;
    json["pass"]=cfg._Password;
    json["IP"]=cfg._Static_IP;
    json["Subnet"]=cfg._SubnetMask;
    json["Gateway"]=cfg._Gateway;
    json["HostName"]=cfg._HostName;
    json["IOT_Server"]=cfg._Server_IP;
    json["Port"]=cfg._PortNumber;
    json["AccessPointConnectingTimeOut"]=cfg._AccessPointConnectingTimeOut;
    json["TOKEN"]=cfg._TOKEN;
    json["SoftAPName"]=cfg._SoftAPName;
    json["SoftAPPassword"]=cfg._SoftAPPassword;

    json["ClientID"]=cfg._ClientID;
    json["Topic"]=cfg._Topic;
    
    
    File configFile = SPIFFS.open("/config.json", "w");
    
    
    if (!configFile) {
     Serial.println("failed to open config file for writing");
    }
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    
}



bool read_config()
{
  //clean FS, for testing
  //SPIFFS.format();
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) 
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) 
    {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) 
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) 
        {
          Serial.println("\nparsed json");
          
          strcpy(cfg._SSID, json["ssid"]);
          strcpy(cfg._Password, json["pass"]);
          strcpy(cfg._Static_IP, json["IP"]);
          strcpy(cfg._SubnetMask, json["Subnet"]);
          strcpy(cfg._Gateway, json["Gateway"]);
          strcpy(cfg._HostName, json["HostName"]);
          strcpy(cfg._Server_IP, json["IOT_Server"]);
          strcpy(cfg._TOKEN, json["TOKEN"]);
          strcpy(cfg._SoftAPName, json["SoftAPName"]);
          strcpy(cfg._SoftAPPassword, json["SoftAPPassword"]);
          strcpy(cfg._ClientID, json["ClientID"]);
          strcpy(cfg._Topic, json["Topic"]);


          
          int i_temp=json["Port"];
          cfg._PortNumber=i_temp;

          i_temp=json["AccessPointConnectingTimeOut"];
          cfg._AccessPointConnectingTimeOut=i_temp;

          Serial.println(cfg._SSID);
          Serial.println(cfg._Password);
          Serial.println(cfg._Static_IP);
          Serial.println(cfg._SubnetMask);
          Serial.println(cfg._Gateway);
          Serial.println(cfg._HostName);
          Serial.println(cfg._Server_IP);
          Serial.println(cfg._PortNumber);
          Serial.println(cfg._AccessPointConnectingTimeOut);
          Serial.println(cfg._TOKEN);
          Serial.println(cfg._SoftAPName);
          Serial.println(cfg._SoftAPPassword);
          Serial.println(cfg._ClientID);
          Serial.println(cfg._Topic);
          
        } else {
          Serial.println("failed to load json config");
          return false;
        }
      }
    }
  } 
  else 
  {
    Serial.println("failed to mount FS");
    return false;
  }
  return true;
  //end read
}
void handleRoot() {
   if (server.hasArg("SSID")&& server.hasArg("Password")&& server.hasArg("IP")){//&&server.hasArg("GW") ) {//If almost all form fields contain data call handelSubmit()
    handleSubmit();
  }
  else {//Redisplay the form
    
    //String s=WebTempToDisplay+MAIN_page;
    
    //server.send(200,"text/html",s);
    
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plane","");

  }
}

void handleSubmit()
{//dispaly values and write to memmory
  server.arg("SSID").toCharArray(cfg._SSID,server.arg("SSID").length()+1);
  server.arg("Password").toCharArray(cfg._Password,server.arg("Password").length()+1);
  server.arg("IP").toCharArray(cfg._Static_IP,server.arg("IP").length()+1);
  server.arg("SubnetMask").toCharArray(cfg._SubnetMask,server.arg("SubnetMask").length()+1);
  server.arg("Gateway").toCharArray(cfg._Gateway,server.arg("Gateway").length()+1);
  server.arg("HostName").toCharArray(cfg._HostName,server.arg("HostName").length()+1);
  server.arg("ServerIP").toCharArray(cfg._Server_IP,server.arg("ServerIP").length()+1);

  server.arg("TOKEN").toCharArray(cfg._TOKEN,server.arg("TOKEN").length()+1);
  server.arg("SoftAPName").toCharArray(cfg._SoftAPName,server.arg("SoftAPName").length()+1);
  server.arg("SoftAPPassword").toCharArray(cfg._SoftAPPassword,server.arg("SoftAPPassword").length()+1);

  server.arg("Topic").toCharArray(cfg._Topic,server.arg("Topic").length()+1);
  server.arg("ClientID").toCharArray(cfg._ClientID,server.arg("ClientID").length()+1);
  
  
  cfg._PortNumber=server.arg("PortNumber").toInt();
  cfg._AccessPointConnectingTimeOut=server.arg("AccessPointConnectingTimeOut").toInt(); 

  write_config(); 
  //displayInBrowser(cfg);

/*        Serial.println(cfg._SSID);
          Serial.println(cfg._Password);
          Serial.println(cfg._Static_IP);
          Serial.println(cfg._SubnetMask);
          Serial.println(cfg._Gateway);
          Serial.println(cfg._Server_IP);
          Serial.println(cfg._PortNumber);
*/
}


bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}
void handleWebRequests(){
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}



IPAddress covertToIP(char *strIP)
{
  String info[4];
  String data=strIP;
  info[0]=getValue(strIP,'.',0);
  info[1]=getValue(strIP,'.',1);
  info[2]=getValue(strIP,'.',2);
  info[3]=getValue(strIP,'.',3);
  IPAddress iIP(info[0].toInt(),info[1].toInt(),info[2].toInt(),info[3].toInt());
  return iIP;
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}




void myWiFiManagerSetup()
{
  char const *ssid=cfg._SSID;
  char const *pass=cfg._Password;

  IPAddress LocalIP;
  IPAddress GW;
  IPAddress SM;
  
  boolean flag=false;
  if(read_config())
  {
    LocalIP=covertToIP(cfg._Static_IP);
    GW=covertToIP(cfg._Gateway);
    SM=covertToIP(cfg._SubnetMask);
    //IPAddress ip(192,168,1,234);         // choose IP address

    WiFi.config(LocalIP,GW,GW,SM);
    //char const *HN=cfg._HostName;
    WiFi.hostname(cfg._HostName);

    

    WiFi.begin(ssid,pass);

  }
 
  for(int i=1;i<cfg._AccessPointConnectingTimeOut;i++)
  {
    if(WiFi.status()!=WL_CONNECTED){
      delay(500);
      Serial.print(".");
      flag=false;
      APisConnected=false;
    }
    else
    {
      Serial.println("");
      Serial.println("Connected To Access Point");  
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());

      Serial.println(covertToIP(cfg._Static_IP));//IPAddress
      WiFi.softAPdisconnect(true);
      flag=true;
      APisConnected=true;
      break;
    }
  }

  if(WiFi.status()!=WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("Setting soft-AP ... ");
     char const *ss=cfg._SoftAPName;
     char const *pa=cfg._SoftAPPassword;
     WiFi.softAPdisconnect(true);

    boolean result = WiFi.softAP(ss, pa);//("SODCO_SoftAP","Password");
    if(result == true)
    {
      Serial.println("Ready");
      Serial.println("Connected");  
      flag=true;
      APisConnected=false;
    }
    else
    {
      Serial.println("Failed in creating softAP!");
      flag=false;
    }
  }


  if(flag)
  {
    
    //Initialize Webserver
    server.on("/",handleRoot);
    server.on("/LoadConfig", LoadConfig);
    server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
    server.begin();  
    Serial.println("Web Server is started!");
  }
  
}


