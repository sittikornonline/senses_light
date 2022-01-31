//------------------------ Setting ------------------//
#define wifi_mng false
bool c_setup_wifi = false;
bool debug_sensor = false;
bool test_max_log = false;

//------------------------ WiFi ------------------//

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

WiFiClient client_wifi;
PubSubClient mqtt_wifi(client_wifi);

//char ssid[30] = "Tenda_815AD0";
//char passw[30] = "qr4DiMDh";

//char ssid[30] = "Senses";
//char passw[30] = "senses69";

char ssid[30] = "BIG_WIFI_2.4G";
char passw[30] = "076434654";

//char ssid[30] = "khoyuiindy@14_2.4G";
//char passw[30] = "22290114";

void setup_wifi();
void reconnect_wifi();
bool state_setup_wifi = false;
byte cnt_wifi_disconect = 0;

String mac_address = "";
bool status_conneect_wifi = false;
bool status_mqtt = false;
bool status_network = false;


//-------------- WiFiManager -------------------//
#include "WiFiManager.h"  // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#ifdef ESP32
#include <SPIFFS.h>
#endif
//WiFiManager wifiManager;
bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm;
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

const char* AP_name  = "Senes_AP";
const char* AP_passw = "1234567890";

#define TRIGGER_PIN 0  // weizen = 0 , senses bock = 39, ayara = 0
#define LED_PIN 2      // weizen = 2 , senses bock = 4(diy)
void led_on();
void led_off();


byte status_wifi = 99;
byte last_status_wifi = 99;

bool status_sta = false;
bool last_status_sta = false;


bool shouldSaveConfig = false;

void checkButton();


//------------------------ MQTT ----------------------//

char*mqtt_broker  = "tracking.sensesiot.com";
int mqtt_port = 3902;
String device_id = "";
String mqtt_pub = "push_";
String mqtt_sub = "ctrl_";

//char mqtt_server[40];
//char mqtt_port[6];
//char device_id[6];
char topic_pub[15];
char topic_sub[15];
char key[15];

String payload_w = "";
bool status_send_w = false;

String setPayload_w();
String setPayload_hb();


//------------------------  Interval ----------------------//



// Interval Other
unsigned int lastMsg_other = 0;
const unsigned int interval_other = 5000;



//----------------------- Task ---------------------------//
TaskHandle_t Task_reconnect_wifi, Task_status_wifi_led, Task_debug_other, Task_serial, Task_sensor, Task_spiff;

void f_Task_reconnect_wifi( void * pvParameters );
void f_Task_status_wifi_led( void * pvParameters );
void f_Task_debug_other( void * pvParameters );
void f_Task_serial( void * pvParameters );
void f_Task_sensor( void * pvParameters );
void f_Task_spiff( void * pvParameters );

void f_get_sensor();

//----------------------- NTPClient ---------------------------//

#include <WiFiUDP.h>
#include <NTPClient.h>
#include "time.h"
const char *NTP_SERVER = "pool.ntp.org";
const int NTP_UPDATE_INTERVAL = 1000;  // ms between NTP queries

WiFiUDP udp;
NTPClient ntpClient(udp, NTP_SERVER, 0, NTP_UPDATE_INTERVAL);
struct tm timeinfo;

unsigned long getTime();
String getTime_str();
String unix_time_str = "";
byte time_operate_sec  = 14;


//----------------------- Other ---------------------------//
bool status_send_payload_w = false;
void initall();


//------------------------ EEPROM ----------------------//
#include "EEPROM.h"
#define EEPROM_SIZE 150
#define add_ssid 0
#define add_passw 30
#define add_unix_time 60
#define add_cnt_payload 90

int cnt_payload = 0;
//------------------------ Serial CMD -------------------//


String inputString = "";
bool stringComplete = false;
void serialEvent();

bool debug_all = true;
bool debug_other = true;
bool flag_putdata = false;

void f_cmd(String data);
void f_debug_other();

void debug(String str);
void debugln(String str);




//------------------------ Senses Light -------------------//

#include "Wire.h"
#define mc_mode 1 // 1 : run , 2 random
#define  FIRMWARE_X    1


//------------- WiFI : MAC Address -------------//
#include <WiFi.h>
String gateway_id = "";
String last_3_mac = "";
uint8_t ble_mac[7];

//----------------- Light : 6030 -------------//
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#define AL_ADDR 0x48
SparkFun_Ambient_Light light_6030(AL_ADDR);
float gain = .125;
int times = 100;
long light_6030_raw = 0;

//----------------- RGB Sensor : 6040 -------------//
#include "veml6040.h"
VEML6040 RGBWSensor;
long light_6040_raw = 0;
uint16_t VEML6040_red;
uint16_t VEML6040_green;
uint16_t VEML6040_blue;
uint16_t VEML6040_white;
/*
   Battery Life Calculator
   https://www.omnicalculator.com/other/battery-life

*/
RTC_DATA_ATTR uint16_t bootCount = 0;
unsigned long startMillis = 0;        // will store last time LED was updated
unsigned long wakeupMillis = 0;
unsigned long sleepMillis = 0;
//----------------- UV : 6075 -------------//
#include <SparkFun_VEML6075_Arduino_Library.h>
VEML6075 uv;
float uv_a_raw;
float uv_b_raw;

#include <MPU6050_light.h>          // https://downloads.arduino.cc/libraries/github.com/rfetick/MPU6050_light-1.2.1.zip
//float temperature;
//float AccX;
//float AccY;
//float AccZ;
//float GyroX;
//float GyroY;
//float GyroZ;
//float AccAngleX;
//float AccAngleY;
//float AngleX;
//float AngleY;
//float AngleZ;
float mpu6050Temps;
float mpu6050Data;
MPU6050 mpu(Wire);
/*
  //----------------- MPU : 6050 -------------//
  #include "I2Cdev.h"
  #include "MPU6050.h"
  // AD0 low = 0x68 (default for InvenSense evaluation board)
  // AD0 high = 0x69
  MPU6050 accelgyro;
  //MPU6050 accelgyro(0x69); // <-- use for AD0 high
*/
/*
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int8_t ax8_t;
  int8_t ay8_t;
  int8_t az8_t;
*/
#include <ESP32AnalogRead.h>      /* https://github.com/madhephaestus/ESP32AnalogRead.git */
ESP32AnalogRead adc;

const int REDLED = 2;
const int GRNLED = 15;
const int BLULED = 4;
const int POW_PIN = 12;
const int CHG_PIN = 27;

uint8_t blink_led_number;
uint8_t Error_code = 0;
const int NORMAL = 0x00;
const int VEML6030 = 0x01;
const int VEML6040 = 0x02;
const int VEML6075 = 0x04;
const int MPU6050 = 0x08;
const int LOWBAT = 0x10;


//----------------- Deep Sleep -------------------//

#include "esp_sleep.h"
#define SLEEP_DURATION_5SEC (5*1000)
#define SLEEP_DURATION_10SEC (10*1000)
#define SLEEP_DURATION_30SEC (30*1000)
#define SLEEP_DURATION_1MIN (60*1000)
#define SLEEP_DURATION_2MIN (120*1000)
#define SLEEP_DURATION_3MIN (180*1000)
#define SLEEP_DURATION_5MIN (300*1000)
#define SLEEP_DURATION_7MIN (420*1000)
#define SLEEP_DURATION_10MIN (600*1000)
#define GPIO_DEEP_SLEEP_DURATION SLEEP_DURATION_30SEC    // For 1 minute
#define time_advers 3     // 3 seconds.
#define ADVERS_TIME_MS (time_advers * 1000)     // 15

bool gyroOffSetFlag = false;
bool lowBattFlag = false;
bool bleAdverFlag = true;
bool battFullFlag = false;


RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;
#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
byte TIME_TO_SLEEP = 60;        //Time ESP32 will go to sleep (in seconds)





//----------------- LED RGB -------------------//
#define red_pin 2
#define green_pin 15
#define blue_pin 4

const int freq = 5000;

const int red_Channel = 0;
const int green_Channel = 1;
const int blue_Channel = 2;
const int resolution = 8;

void led_rgb(int r, int g, int b);

//----------------- V-Batt -------------------//
#define v_batt_pin 39
int v_batt = 0;
int v_batt_map = 0;
float batt_voltage = 0;
#define batt_pin 27 // 0 = full
int batt_full = 0;

void led_batt_full();
void led_batt_low();

//----------------- Other -------------------//

void f_init_all();
void set_payload();



//----------------- Converse -------------------//
char payload_mac[6];
char payload_beacon[30];
byte payload_beacon_length = 0;
char* str2charArray(String str) ;
long x2i(char *s);
void mac_to_byte_array();
void ble_mac2String();
//String ble_mac2String();
void printDeviceAddress();
//----------------- Power sensor off -----------//
#define power_sensor_pin 12


//----------------- SPIFF ----------------------//
#include "senses_spiff.h"

void putData();
void shiftData();
void writeShitf(String dataFilename);
void unshiftData();
void lastData();
void cmd();

senses_spiff spiff;

String payload_str = "";
String payload_mqtt = "";
String ends = "\n";

bool emptyFile = false;
bool emptyFile_2 = false;
bool emptyFile_3 = false;
bool emptyFile_4 = false;


#define min_file_1 1
#define max_file_1 250

#define min_file_2 251
#define max_file_2 500

#define min_file_3 501
#define max_file_3 750


//#define min_file_1 1
//#define max_file_1 5
//
//#define min_file_2 6
//#define max_file_2 10
//
//#define min_file_3 11
//#define max_file_3 15

byte code_cmd_spiff = 0;
bool flag_spiff = false;
String test_larg_payload();

//----------------- RTC - BuildIn ----------------------//
bool flag_upload_when_night = true;
