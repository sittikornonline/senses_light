#include "Wire.h"
#define mc_mode 1 // 1 : run , 2 random
#define  FIRMWARE_X    1


//------------- WiFI : MAC Address -------------//
#include <WiFi.h>
String mac_address = "";
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
 * Battery Life Calculator
 * https://www.omnicalculator.com/other/battery-life
 * 
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
//----------------- BLE -------------------//

#include "sys/time.h"

#include <Arduino.h>

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include "BLEAdvertising.h"
#include "BLEEddystoneURL.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#define BEACON_UUID "8ec76ea3-6668-48da-9866-75be8bc86f4d"

uint16_t beconUUID = 0xFEAA;
void setBeacon();

String ble_address = "";
char str[3];
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
byte cnt_advers = 1;

RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;

BLEAdvertising *pAdvertising;
//struct timeval nowTimeStruct;

//time_t lastTenth;



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
