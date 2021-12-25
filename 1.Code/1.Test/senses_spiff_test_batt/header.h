// Open Serial Monitor: Buad rate = 115200,Newline

#include "senses_spiff.h"

void putData(int _brewtime);
void shiftData();
void writeShitf(String dataFilename);
void unshiftData();
void lastData();
void cmd();

senses_spiff spiff;



int pwrnum = 0;
int brewnum = 0;
int brewtime = 0;

int cnt = 0;
int brewcnt = 0;

String inputString = "";
bool stringComplete = false;

String ends = "\n";
bool emptyFile = false;
byte no_of_data = 3;

unsigned long previousMillis = 0;
const long interval = 1000;

//---------------------------------------------//

#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30        /* Time ESP32 will go to sleep (in seconds) */


//-----------------------------
#include "EEPROM.h" 

//-----------------------------

#define batt_pin 27 // logic 0 = full
#define red_pin 2
#define green_pin 15
#define blue_pin 4
