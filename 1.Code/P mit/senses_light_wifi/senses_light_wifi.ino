#include <stdio.h>
#include "header.h"
#include "Arduino.h"
/*
 * 03-12-2021 : Change battery full charge state indicator -> Green LED allway ON.
 */

/* start 21.10 - 21-11-2021
-------------------------  WAKEUP TIME  ---------------------------
WIFI_ADDRESS : 308398D9FD54
GATEWAY_ID   : SEN_D54
6030 AL      : 617
6040 R       : 11213
6040 G       : 11967
6040 B       : 5966
6040 W       : 35631
6075 UVA     : 47.28
6075 UVB     : 86.10
TEMPERATURE  : 40.46
BATT V       : 4.20 V
ERROR CODE   : 0x00
BLE PAYLOAD  : 02:69:2B:CD:2E:BF:17:4E:8B:2F:12:78:21:A2:0F:CD:00
WAKEUP TIME  : 7281
SLEEP TIME   : 172719
-----------------------------  END  -------------------------------

 */

// Util Functions
void blink(uint8_t count, uint8_t pin) {
  uint8_t state = HIGH;

  for (int x = 0; x < (count << 1); ++x) {
    digitalWrite(pin, state ^= HIGH);
    delay(75);
  }
  digitalWrite(pin, HIGH);
}

void setup() {
//  f_init_all();
  startMillis = millis();

#if 1
  esp_reset_reason_t reason = esp_reset_reason();

  /* Reset when the vdd voltage is not stable */
  if (reason == ESP_RST_BROWNOUT){
    esp_deep_sleep(1000000LL * 3600000);    // long time sleep 1 hour.

  } else if (reason == ESP_RST_POWERON){
    gyroOffSetFlag = true;
  }

#endif  
}
void loop() {
  /* Clear ERROR cade */
  Error_code = 0;
  lowBattFlag = false;
  //Increment boot number and print it every reboot
  ++bootCount;
  adc.attach(39);

  /*
   * Initial GPIO to OUTPUT and Turn Sensor Power supply. and turn OFF all RGB LED.
   * 
   */
  pinMode(CHG_PIN, INPUT);
  digitalWrite(POW_PIN, HIGH);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GRNLED, HIGH);
  digitalWrite(BLULED, HIGH);

  pinMode(POW_PIN, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(GRNLED, OUTPUT);
  pinMode(BLULED, OUTPUT);

  /*
   * Initialize serial 115200.   
   */
  Serial.begin(115200);
  Serial.println("\n---------------------------  WAKEUP  ------------------------------");
  Serial.println("CYCLE NUMBER : " + String(bootCount));
  /* Power stabilize time 100 ms. */
  delay(100);

 

/*
 * 
 * RGB LED Status
 * if VBATT > 3.60V & Battery charge Full - All way On Green LED.
 * if VBATT > 3.60V - Blink Blue LED 2 times.
 * if VBATT > 3.50V - Blink Red LED 2 times.
 * if VBATT > 3.45V - Blink Red LED 1 times.
 * if VBATT < 3.45V - Blink Red LED 1 times and set LOWBATT in Error code.
 * 
 * VDD supply voltage range for sensers.
 * VEML6030 : Operation voltage: 2.5 V to 3.6 V
 * VEML6040 : Operation voltage: 2.5 V to 3.6 V
 * VEML6075 : Operation voltage: 1.7 V to 3.6 V
 * MPU6050  : Operation voltage: 2.37V to 3.46V
 * ESP32-PICO-D4  : Operation voltage: 3.0 V to 3.6 V
 *  
 */
  batt_voltage = adc.readVoltage()* 2;
#if 0
  batt_voltage >= 4.150 ? battFullFlag = true : battFullFlag = false;
#else
  if (batt_voltage > 4.150 && !digitalRead(CHG_PIN)){
    battFullFlag = true;
  } else {
    battFullFlag = false;
  }
#endif
  if(!battFullFlag){
//  if (digitalRead(CHG_PIN)== HIGH && batt_voltage <= 4.10) {
  
    if (batt_voltage >= 3.70) {
      bleAdverFlag ? blink(2, GRNLED) : blink(1, GRNLED);
    } else if (batt_voltage >= 3.60) {
      /* if battery > 3.60V Blink Green LED */
//      if (digitalRead(CHG_PIN)) { /* if battery Not full */
        bleAdverFlag ? blink(2, BLULED) : blink(1, GRNLED);
//      } else {
//        digitalWrite(GRNLED, HIGH);   // turn OFF before sensor reading process.
        /* blink(3, GRNLED);       */
//      }
    } else if (batt_voltage > 3.50) {
      /* if battery > 3.50V Blink Red LED */
      bleAdverFlag ? blink(2, REDLED) : blink(1, GRNLED);    
    } else if (batt_voltage > 3.45){
      /* if battery > 3.45V Blink Red LED */
      bleAdverFlag ? blink(1, REDLED) : blink(1, GRNLED);    
    } else {
      /* if battery < 3.45V Blink Red LED and set Low battery bits. */
      Error_code |= LOWBAT;
      lowBattFlag = true;
      blink(1, REDLED);        
    }
  }
/*
  Serial.println("Voltage = "+String(adc.readVoltage()* 2)+"V");
  delay(5000);
*/

  //----------------- i2C -------------//
  Wire.begin (21, 22);
  Wire1.begin (26, 25);               // Wire1 for VEML6040
  
  //----------------- Light : 6030 -------------//
  if(!light_6030.begin()) {
    Error_code |= VEML6030;
  }

  //----------------- Light : 6040 -------------//
  if(!RGBWSensor.begin()) {
    Error_code |= VEML6040;
  }

  //----------------- UV : 6075 -------------//
  if (!uv.begin()){
    Error_code |= VEML6075;
  }

  //----------------- MPU : 6050 -------------//
  if (mpu.begin()){
    Error_code |= MPU6050;
  }

  delay(100);
  light_6030.setGain(gain);
  light_6030.setIntegTime(times);
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
  
  /* Only ESP_RST_POWERON case. MPU6050 run calcOffsets function*/
  if (gyroOffSetFlag) {
    mpu.calcOffsets(true,true); // gyro and accelero
    gyroOffSetFlag = false;
    delay(1500);  
    mpu.update();
    mpu6050Temps = mpu.getAccAngleX();
  }
  /* Setup time delay for all sensor */
  delay(1500);
  
  /* If bleAdverFlag flag is SET. then read all sensor data. */
  if (bleAdverFlag) {
    mpu.update();
  //  mpu6050Data = mpu.getTemp();        //  temperature
    mpu6050Data = mpu.getAccX();        //  AccX
  //  mpu6050Data = mpu.getAccY();        //  AccY
  //  mpu6050Data = mpu.getAccZ();        //  AccZ
  //  mpu6050Data = mpu.getGyroX();       //  GyroX
  //  mpu6050Data = mpu.getGyroY();       //  GyroY
  //  mpu6050Data = mpu.getGyroZ();       //  GyroZ
  //  mpu6050Data = mpu.getAccAngleX();     //  AccAngleX
  //  mpu6050Data = mpu.getAccAngleY();   //  AccAngleY
  //  mpu6050Data = mpu.getAngleX();      //  AngleX
  //  mpu6050Data = mpu.getAngleY();      //  AngleY
  //  mpu6050Data = mpu.getAngleZ();      //  AngleZ
    
//    light_6030_raw = light_6030.readLight();
  
    VEML6040_red = RGBWSensor.getRed();
    VEML6040_green = RGBWSensor.getGreen();
    VEML6040_blue = RGBWSensor.getBlue();
    VEML6040_white = RGBWSensor.getWhite();
  
    uv_a_raw = (uv.uva() * 100);       // function uv.uva RETURN folting point
    uv_b_raw = (uv.uvb() * 100);
  }
  light_6030_raw = light_6030.readLight();
/*********************************************************************************** SENSOR POWER OFF ****************************************************************************************************/
  digitalWrite(POW_PIN, LOW);
  
  if (battFullFlag) {
    digitalWrite(GRNLED, LOW);
    delay(1000);
    digitalWrite(GRNLED, HIGH);
  }

#if 1
  /* If Ambient_Light <= 3 then Not send BLE data */
  if(light_6030_raw <= 3) {
    bleAdverFlag = false;
  } else {
    bleAdverFlag = true;    
  }
#endif


#if 0
  //------------- WiFI : MAC Address -------------//
  mac_address = WiFi.macAddress();
  mac_address.replace(":", "");
  last_3_mac = mac_address.substring(9, 12);
  gateway_id = "SEN_" + last_3_mac;
  mac_to_byte_array();

#endif
  if(bleAdverFlag) {
    /*------------- BLE : MAC Address -------------*/
  
    /* Create the BLE Device with Blank name. */
    BLEDevice::init("");
  
    const uint8_t* point = esp_bt_dev_get_address();
    /* Get BLE MAC Address */
    for (int i = 0; i < 6; i++) {
      sprintf(str, "%02X", (int)point[i]);
      ble_address += str; 
    }
    /* Set device name. */
    last_3_mac = ble_address.substring(9, 12);
    gateway_id = "SEN_" + last_3_mac;
    
    /* Create the BLE Device with device name. */
    BLEDevice::init(gateway_id.c_str());
    BLEDevice::setPower(ESP_PWR_LVL_N12);   /* Bluetooth TX power level */
  
    pAdvertising = BLEDevice::getAdvertising();
  
    //Advers 1
    set_payload();
    setBeacon();
    pAdvertising->start();
    delay(ADVERS_TIME_MS);
#if 0
    if (battFullFlag) {
      delay(SLEEP_DURATION_5SEC - ADVERS_TIME_MS);
    }
#endif
  }
/* ESP32 BOOT MESSAGE 
ets Jun  8 2016 00:22:57 = 26 byte

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT) = 58 BYTE = 68
configsip: 188777542, SPIWP:0xee = 33 BYTE = 44
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00 = 73
mode:DIO, clock div:1 = 23
load:0x3fff0018,len:4 = 23
load:0x3fff001c,len:1216 = 26
ho 0 tail 12 room 4 = 21
load:0x40078000,len:10944 = 27
load:0x40080400,len:6388 = 26
entry 0x400806b4 = 18
TOTAL = 375 BYTE = +/- 26 MSEC
  = 1/115200 = 8.680555555555556e-6 * 8 = 6.944444444444444e-5 / BYTE
  = 6.944444444444444e-5 * 375 = 0.0260416666666667 = 26.04 MSEC
 
*/
#define BOOT_MESSAGE_TIME 26

  wakeupMillis = millis() - startMillis;
  wakeupMillis = wakeupMillis + BOOT_MESSAGE_TIME;
  
  /* if Low battery state set 10 minute time period for send data.  */
  if (Error_code & LOWBAT ) {
    sleepMillis +=  (GPIO_DEEP_SLEEP_DURATION + SLEEP_DURATION_7MIN) - wakeupMillis;
  } else {
    sleepMillis = GPIO_DEEP_SLEEP_DURATION - wakeupMillis;
  }
  Serial.print("WAKEUP TIME  : "); Serial.println(wakeupMillis);
//  gettimeofday(&nowTimeStruct, NULL);
  Serial.print("SLEEP TIME   : "); Serial.println(sleepMillis);
  Serial.println("---------------------------  SLEEP  -------------------------------");

//  last = nowTimeStruct.tv_sec;
//  lastTenth = nowTimeStruct.tv_sec * 10; // Time since last reset as 0.1 second resolution counter
#if 1
  esp_deep_sleep(1000LL * sleepMillis);  
#else
  if (!digitalRead(CHG_PIN)) {
    delay(sleepMillis);
  } else {
    esp_deep_sleep(1000LL * sleepMillis);
  }
#endif
}
