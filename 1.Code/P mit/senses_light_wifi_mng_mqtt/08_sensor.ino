void f_get_sensor()
{
  /* Clear ERROR cade */
  Error_code = 0;
  lowBattFlag = false;
  //Increment boot number and print it every reboot
  ++bootCount;
  adc.attach(39);


  pinMode(CHG_PIN, INPUT);
  digitalWrite(POW_PIN, HIGH);
  digitalWrite(REDLED, HIGH);
  digitalWrite(GRNLED, HIGH);
  digitalWrite(BLULED, HIGH);

  pinMode(POW_PIN, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(GRNLED, OUTPUT);
  pinMode(BLULED, OUTPUT);



  //----------------- i2C -------------//
  Wire.begin (21, 22);
  Wire1.begin (26, 25);               // Wire1 for VEML6040

  //----------------- Light : 6030 -------------//
  if (!light_6030.begin()) {
    Error_code |= VEML6030;
  }

  //----------------- Light : 6040 -------------//
  if (!RGBWSensor.begin()) {
    Error_code |= VEML6040;
  }

  //----------------- UV : 6075 -------------//
  if (!uv.begin()) {
    Error_code |= VEML6075;
  }

  //----------------- MPU : 6050 -------------//
  if (mpu.begin()) {
    Error_code |= MPU6050;
  }

  delay(100);
  light_6030.setGain(gain);
  light_6030.setIntegTime(times);
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);



  batt_voltage = adc.readVoltage() * 2;
#if 0
  batt_voltage >= 4.150 ? battFullFlag = true : battFullFlag = false;
#else
  if (batt_voltage > 4.150 && !digitalRead(CHG_PIN)) {
    battFullFlag = true;
  } else {
    battFullFlag = false;
  }
#endif
  if (!battFullFlag) {
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
    } else if (batt_voltage > 3.45) {
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



  /* Only ESP_RST_POWERON case. MPU6050 run calcOffsets function*/
  if (gyroOffSetFlag) {
    mpu.calcOffsets(true, true); // gyro and accelero
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
  if (light_6030_raw <= 3) {
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




  //#define BOOT_MESSAGE_TIME 26
  //
  //  wakeupMillis = millis() - startMillis;
  //  wakeupMillis = wakeupMillis + BOOT_MESSAGE_TIME;
  //
  //  /* if Low battery state set 10 minute time period for send data.  */
  //  if (Error_code & LOWBAT ) {
  //    sleepMillis +=  (GPIO_DEEP_SLEEP_DURATION + SLEEP_DURATION_7MIN) - wakeupMillis;
  //  } else {
  //    sleepMillis = GPIO_DEEP_SLEEP_DURATION - wakeupMillis;
  //  }


  float uv_a = uv_a_raw / 100;
  float uv_b = uv_b_raw / 100;

  byte min_of_check = 20;

  if ((light_6030_raw  <= min_of_check) || (light_6030_raw  == 255) &&
      VEML6040_red <= min_of_check &&
      VEML6040_green  <= min_of_check &&
      VEML6040_blue   <= min_of_check &&
      VEML6040_white   <= min_of_check &&
      uv_a   <= min_of_check &&
      uv_b   <= min_of_check  )
  {
    flag_upload_when_night = true;
    c_setup_wifi = true;
  }

  else {
    flag_upload_when_night = false;
    c_setup_wifi = false;
  }

  Serial.print("flag_upload_when_night : "); Serial.println(flag_upload_when_night);




}
