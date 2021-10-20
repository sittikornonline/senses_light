#define c_spiff true
#define wifi true
#define esp32 true
#define esp8266 false
#define dump true
#define mqtt true
#define times true
#define serial true

bool state_list_file = false;
bool state_initall = false;



#if esp32
#include <WiFi.h>
#include <WebServer.h>
#include "senses_spiff.h"
#include "FS.h"
senses_spiff spiff;
#define FORMAT_SPIFFS_IF_FAILED true
#endif


#if esp8266
#include <LittleFS.h>
FS* filesystem = &LittleFS;
#define FileFS  LittleFS
#define FS_Name "LittleFS"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

#if mqtt

#include <WiFiClient.h>
#include <PubSubClient.h>

/* ---------- MQTT parameters --------*/

const char* mqtt_server = "tracking.sensesiot.com"; //.. UTILITY VM.
WiFiClient espClient;
PubSubClient mqtt_wifi(espClient);

bool status_mqtt = false;

#define MSG_BUFFER_SIZE (1000)
char msg[MSG_BUFFER_SIZE];

int mqtt_port = 1883;
String device_id = "senses_light_01";
String mqtt_pub = "senses_light_pub" ;
String mqtt_sub = "senses_light_sub" ;

String topics_str = "push_" + WiFi.macAddress();
const char* topics = topics_str.c_str();

String ctrl_topics_str = "ctrl_" + WiFi.macAddress();
const char* ctrl_topics = ctrl_topics_str.c_str();

String msg_str = "";
String msg_cmd = "";

char *mqtt_broker = "";
char *_mqtt_broker = "";


#endif
/* ---------------------------------- */

#if wifi
unsigned long current_millis;
unsigned long offline_rec_interval = 1;  //..sec.
unsigned long offline_rec_millis;

const char* ssid = "boom";
const char* passw = "1234567890";
bool state_setup_wifi = false;
byte cnt_wifi_disconect = 0;
bool status_network = false;

#include <DNSServer.h>
#define UPLOAD_HOST "tracking.sensesiot.com" //.. UTILITY VM.
#define UPLOAD_PORT 13000

#define BOUNDARY "----5923c593ca3ddf31ad7b6"
#define TIMEOUT 5000
#include <WiFiUdp.h>
#include <NTPClient.h>
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


#endif


//----------------------- Task ---------------------------//

TaskHandle_t Task_dump, Task_Buzzer, Task_Reconnect_wifi, Task_serial, Task_list_file;

void f_Task_dump( void * pvParameters );
void f_Task_Reconnect_wifi( void * pvParameters );
void f_Task_serial( void * pvParameters );
void f_Task_list_file( void * pvParameters );

void initall();
void serialEvent();
void run_work();
void cmd_serial();
void cmd_str(String inputString);
void list_file();


//----------------------- Manual SPIFF ---------------------------//
int pwrnum = 0;
int brewnum = 0;
int brewtime = 0;

int cnt = 0;
int brewcnt = 0;


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String ends = "\n";
bool emptyFile = false;
byte no_of_data = 3;

unsigned long previousMillis = 0;
const long interval = 1000;

void putData(int _brewtime);
void shiftData();
void writeShitf(String dataFilename);
void unshiftData();
void lastData();
void cmd();
