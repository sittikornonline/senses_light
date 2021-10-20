#include <LittleFS.h>
FS* filesystem = &LittleFS;
#define FileFS  LittleFS
#define FS_Name "LittleFS"

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#define UPLOAD_HOST "tracking.sensesiot.com" //.. UTILITY VM.
#define UPLOAD_PORT 13000

#define BOUNDARY "----5923c593ca3ddf31ad7b6"
#define TIMEOUT 5000

const char* ssid = "wlan-secure";
const char* passw = "4502160000";

/* ---------- MQTT parameters --------*/
const char* mqtt_server = "tracking.sensesiot.com"; //.. UTILITY VM.
WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE (1000)
char msg[MSG_BUFFER_SIZE];

String topics_str = "push_" + WiFi.macAddress();
const char* topics = topics_str.c_str();
String ctrl_topics_str = "ctrl_" + WiFi.macAddress();
const char* ctrl_topics = ctrl_topics_str.c_str();

String msg_str = "";
String msg_cmd = "";
/* ---------------------------------- */

unsigned long current_millis;
unsigned long offline_rec_interval = 1;  //..sec.  
unsigned long offline_rec_millis;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long epochTime; 
unsigned long uploadUnixTime;

unsigned long filerun_cnt = 0;
String offline_rec_filename = "offline_rec.csv";

String dummy_data = "1111111111111111111111111111111111111111111111111111";
const int txtsizeperfile = 10000;  
char text[txtsizeperfile];
const int offset_write_EOL = 100;

/* ####################### SUB Function ####################### */
void reconnect() {
  String clientId = "client:" + WiFi.macAddress();
  if(client.connect(clientId.c_str())){
    client.publish("reconnected", clientId.c_str());
    client.subscribe(ctrl_topics);
  }else{
    // do nothing..
  }
}

/* MQTT received message callback */
void callback(char* topic, byte* payload, unsigned int length){

  msg_cmd = "";
  for(int i=0; i<length; i++) {
    msg_cmd += (char)payload[i];
  }
  Serial.println("msg_cmd = " + String(msg_cmd));

  /* -- List file -- */
  if(msg_cmd == "ls"){
    pushmessage(listDir("/"));
  }else{

    // -- Checking for other mode --
    String _delmode = "del:";
    for(int i=0; i<msg_cmd.length(); i++){
      if(msg_cmd.substring(i,i+_delmode.length()) == _delmode){
        String _targetDelFile = msg_cmd.substring(i+_delmode.length(), msg_cmd.length());
        Serial.println("_targetDelFile = " + _targetDelFile);
        if(deleteFile(_targetDelFile.c_str())){
          Serial.println("removed ok");
        }else{
          Serial.println("remove failed");
        }
        return;
      }else{
        //..
      }
    }

    String _dumpmode = "dump:";
    for(int i=0; i<msg_cmd.length(); i++){
      if(msg_cmd.substring(i,i+_dumpmode.length()) == _dumpmode){
        String _targetDumpFile = msg_cmd.substring(i+_dumpmode.length(), msg_cmd.length());
        Serial.println("_targetDumpFile = " + _targetDumpFile);
        dumpFile(_targetDumpFile);
        return;
      }else{
        //..
      }
    }
    
  }

}

void pushmessage(String _payload){
  _payload.toCharArray(msg, MSG_BUFFER_SIZE);
  client.publish(topics, msg);  
}

unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

/* PULL these function from PIC Firmware : 23Mar2021, version 8.0 */
String listDir(const char * dirname) {
  String _readDir = "lsfile:";

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    _readDir += root.fileName();
    _readDir += "=";
    _readDir += String(root.fileSize());
    _readDir += "|";
    file.close();
  }
  return _readDir;
}

bool writeFile(const char * path, const char * message) {

  File file = LittleFS.open(path, "w");
  if (!file) {
    return false;
  }
  if (file.print(message)) {
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}

bool write_EOL_File(const char * path, const char * message) {

  File file = LittleFS.open(path, "a");
  if (!file) {
    return false;
  }
  unsigned long limitsize = txtsizeperfile - offset_write_EOL;
  if (file.size() > limitsize){
    Serial.println("Full size of offline file.");
    return false;
  }
  if (file.print(message)) {
    file.close();
    return true;
  } else {
    file.close();
    return false;
  }
}

String readFile(const char * path) {
  String _readStr = "";
  
  File file = LittleFS.open(path, "r");
  if (!file) {
    return "";
  }

  while (file.available()) {
    _readStr += char(file.read());
  }

  file.close();
  return _readStr;
}

bool deleteFile(const char * path) {
  if (LittleFS.remove(path)) {
    return true;
  } else {
    return false;
  }
}

String set_header(size_t length){

  String  data;

      data =  F("POST /offlinelog HTTP/1.1\r\n");
      data += F("Content-Type: multipart/form-data; boundary=");
      data += BOUNDARY;
      data += "\r\n";
      data += F("Host: ");
      data += UPLOAD_HOST;
      data += F(":");
      data += UPLOAD_PORT;
      data += F("\r\n");
      data += F("accept-encoding: utf-8\r\n");
      data += F("Connection: keep-alive\r\n");
      data += F("content-length: ");
      data += String(length);
      data += "\r\n";
      data += "\r\n";

    Serial.println(data);

    return(data);
}

String body(String content , String message){

  String data;
  data = "--";
  data += BOUNDARY;
  data += F("\r\n");
  if(content=="file")
  {
    data += "Content-Disposition: form-data; name=\"offlinelog\"; filename=\"" + message + "\"\r\n";
    data += "Content-Type: text/plain\r\n";
    data += "\r\n";
  }
  else
  {
    data += "Content-Disposition: form-data; name=\"" + content +"\"\r\n";
    data += "\r\n";
    data += message;
    data += "\r\n";
  }

  Serial.println(data);

   return(data);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passw);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void dumpFile(String _filename){
  
  /* -- Upload Offline file to server -- */
  size_t _uploadFileSize;
  File uploadFile = LittleFS.open(_filename.c_str(), "r");
    
  _uploadFileSize = uploadFile.size();
  Serial.println("Start Upload offline file len = " + String(_uploadFileSize) + " bytes.");

  /* -- Check size of dump file -- */
  if(_uploadFileSize >= txtsizeperfile){
    
    Serial.println("Cannot dump file because of size is over");
    pushmessage("file size is over " + String(txtsizeperfile) + " bytes.");
  
  }else{
    
    unsigned long _cnt = 0;
    
    while (uploadFile.available()) {
      text[_cnt] = char(uploadFile.read());
      _cnt++;
    }
    
    uploadFile.close();

    Serial.println("text size = " + String(_cnt));

    String bodyTxt = body("wfmcaddr",WiFi.macAddress().c_str());
    uploadUnixTime = getTime();
  
    String uploadFilename = WiFi.macAddress() + "_" + String(uploadUnixTime) + ".csv";
    uploadFilename.replace(":","");
  
    String bodyFile = body("file", uploadFilename.c_str());
    String bodyEnd =  String("--") + BOUNDARY + String("--\r\n");

    size_t allLen = bodyTxt.length() + bodyFile.length() + _cnt + bodyEnd.length();
    String headerTxt =  set_header(allLen);

    WiFiClient client;
    Serial.println("WiFiClient processing..");
    
    if (!client.connect(UPLOAD_HOST, UPLOAD_PORT)){
      Serial.println("connection failed");
    }else{

      Serial.println("host connected !");
      
      client.print(headerTxt + bodyTxt + bodyFile);
      client.write(text, _cnt);
      client.print("\r\n" + bodyEnd);

      delay(20);
      String serverRes = "";  
      long tOut = millis() + TIMEOUT;
      while(client.connected() && tOut > millis()){
        if (client.available()){
          serverRes = client.readStringUntil('\r');
        }else{
          //..
        }
      }
      Serial.println("serverRes = " + serverRes);
          
    }
  }
  
}

void setup(){

  offline_rec_millis = millis();

  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  
  Serial.begin(115200); 
  while(!Serial){
    //..
  }
 
  delay(1000);
  Serial.println("Test OFFLINE record data into FS..");

  Serial.println(); 
  Serial.println("Initializing FileFS.begin().."); 
  FileFS.begin();

  delay(1000);

  Serial.println();
  Serial.println("List dir..");
  Serial.println(listDir("/"));
  
  Serial.println();
  Serial.println("Check info FS..");
  FSInfo fs_info;
  FileFS.info(fs_info);
  Serial.println("totalBytes = " + String(fs_info.totalBytes) + " bytes.");
  Serial.println("usedBytes = " + String(fs_info.usedBytes) + " bytes.");
  Serial.println("blockSize = " + String(fs_info.blockSize) + " bytes.");
  Serial.println("pageSize = " + String(fs_info.pageSize) + " bytes.");
  Serial.println("maxOpenFiles = " + String(fs_info.maxOpenFiles) + " bytes.");
  Serial.println("maxPathLength = " + String(fs_info.maxPathLength) + " bytes.");

  delay(1000);

  Serial.println(); 
  Serial.println("RE-List dir..");
  Serial.println(listDir("/"));

  initWiFi();
  timeClient.begin();

  client.setBufferSize(2000);             /* Size of sending message in BYTE */
  client.setKeepAlive(15);                /* in Sec., Default is 15 sec. */
  client.setServer(mqtt_server, 1883);    /* Connect to MQTT */
  client.setCallback(callback);

  /* Pre-connection to MQTT */
  if (!client.connected()){
    reconnect();
  }
  
}

void loop(){
  //..
  current_millis = millis();

  /* Recheck connection MQTT */
  if (!client.connected()){ 
    reconnect();
  }
  client.loop();

  /* EVENT 8 : Offline - Record log */
  if((current_millis-offline_rec_millis) >= offline_rec_interval*1000){
    
    //.. do something about log txt record..
    if(WiFi.status() == WL_CONNECTED){
    
      // -- ONLINE --
      digitalWrite(16, LOW);
      epochTime = getTime();
      Serial.println("ONLINE / epochTime = " + String(epochTime));
  
    }else{
  
      // -- OFFLINE --
      digitalWrite(16, HIGH);
      FSInfo fs_info;
      FileFS.info(fs_info);
      String _millis_str = String(getTime());
            _millis_str += "," + dummy_data;
            _millis_str += "," + String(fs_info.usedBytes);
            _millis_str += "\n";
      Serial.println(_millis_str);
     
      bool offline_rec_flag = write_EOL_File(offline_rec_filename.c_str(), _millis_str.c_str());
      if(offline_rec_flag){
        Serial.println("OFFLINE / record completed");
      }else{
        Serial.println("OFFLINE / record failed");
      }
        
    }
  
    offline_rec_millis = millis();
    Serial.flush();
  
  }else{
    //..
  }

}
