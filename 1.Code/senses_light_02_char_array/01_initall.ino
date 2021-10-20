void f_init_all()
{
  Serial.begin(115200);

  //------------- WiFI : MAC Address -------------//

  mac_address = WiFi.macAddress();
  mac_address.replace(":", "");
  gateway_id = "SEN_" + mac_address.substring(9, 12);

  //----------------- i2C -------------//
  Wire.begin (21, 22);
  Wire1.begin (26, 25);

  //----------------- Light : 6030 -------------//
  light_6030.begin();
  light_6030.setGain(gain);
  light_6030.setIntegTime(times);

  if (!RGBWSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
  }

  //----------------- Light : 6040 -------------//
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);


  //----------------- UV : 6075 -------------//
  uv.begin();


  //----------------- MPU : 6050 -------------//
  accelgyro.initialize();


  //----------------- LED RGB -------------------//
  ledcSetup(red_Channel, freq, resolution);
  ledcSetup(green_Channel, freq, resolution);
  ledcSetup(blue_Channel, freq, resolution);


  ledcAttachPin(red_pin,   red_Channel);
  ledcAttachPin(green_pin, green_Channel);
  ledcAttachPin(blue_pin,  blue_Channel);


  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  led_rgb(255, 255, 255);

  //----------------- V-Batt -------------------//

  analogReadResolution(10); 
  pinMode(batt_pin, OUTPUT);

  


}
