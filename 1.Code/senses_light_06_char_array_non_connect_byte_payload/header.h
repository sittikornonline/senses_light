#include "Wire.h"
#define mc_mode 1 // 1 : run , 2 random


//------------- WiFI : MAC Address -------------//
#include <WiFi.h>
String mac_address = "";
String gateway_id = "";
String last_3_mac = "";


//----------------- Light : 6030 -------------//
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#define AL_ADDR 0x48
SparkFun_Ambient_Light light_6030(AL_ADDR);
float gain = .125;
int times = 100;
long light_6030_raw = 0;

//----------------- Light : 6040 -------------//
#include "veml6040.h"
VEML6040 RGBWSensor;
long light_6040_raw = 0;

//----------------- UV : 6075 -------------//
#include <SparkFun_VEML6075_Arduino_Library.h>
VEML6075 uv;
long uv_a_raw = 0;
long uv_b_raw = 0;


//----------------- MPU : 6050 -------------//
#include "I2Cdev.h"
#include "MPU6050.h"
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;



//----------------- BLE -------------------//

#include "sys/time.h"

#include <Arduino.h>

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include "BLEAdvertising.h"
#include "BLEEddystoneURL.h"

#define BEACON_UUID "8ec76ea3-6668-48da-9866-75be8bc86f4d"

uint16_t beconUUID = 0xFEAA;
void setBeacon();

//----------------- Deep Sleep -------------------//

#include "esp_sleep.h"
#define GPIO_DEEP_SLEEP_DURATION 50
#define time_advers 15
byte cnt_advers = 1;

RTC_DATA_ATTR static time_t last;
RTC_DATA_ATTR static uint32_t bootcount;

BLEAdvertising *pAdvertising;
struct timeval nowTimeStruct;

time_t lastTenth;



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

//----------------- Power sensor off -----------//
#define power_sensor_pin 12
