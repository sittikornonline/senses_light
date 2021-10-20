#include "Wire.h"

#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#define AL_ADDR 0x48
SparkFun_Ambient_Light light_6030(AL_ADDR);
float gain = .125;
int times = 100;
long light_6030_raw = 0;


#include "veml6040.h"
VEML6040 RGBWSensor;
long light_6040_raw = 0;


#include <SparkFun_VEML6075_Arduino_Library.h>
VEML6075 uv;
long uv_a_raw = 0;
long uv_b_raw = 0;


void setup() {
  Serial.begin(115200);


  //6030
  light_6030.begin();
  light_6030.setGain(gain);
  light_6030.setIntegTime(times);


  Wire1.begin (26, 25);
  if (!RGBWSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
    //while (1) {}
  }
  //6040
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);


  //6075
  uv.begin();

 
}

void loop() {

  light_6030_raw = light_6030.readLight();
  light_6040_raw = RGBWSensor.getAmbientLight();
  uv_a_raw = uv.uva();
  uv_b_raw = uv.uvb();

  Serial.print("6030 AL  : "); Serial.print(light_6030_raw); Serial.println(" lux");
  Serial.print("6040 AL  : "); Serial.print(light_6040_raw); Serial.println(" lux");
  Serial.print("6075 UVA : "); Serial.println(uv_a_raw);
  Serial.print("6075 UVB : "); Serial.println(uv_b_raw);
  Serial.println("--------------------------");



  delay(4000);
  ESP.restart();
}
