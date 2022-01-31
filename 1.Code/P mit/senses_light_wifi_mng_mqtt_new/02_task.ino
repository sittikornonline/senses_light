
void f_Task_reconnect_wifi( void * pvParameters ) {
  debug("Task_Reconnect_wifi running on core ");
  debugln(String(xPortGetCoreID()));


  for (;;) {

    if (wm.status_sta == false)
    {

      if (status_mqtt == true )
      {
        sendPayload();
      }

      if (state_setup_wifi == false && c_setup_wifi == true )
      {
        setup_wifi();
      }

      else {
        if (!mqtt_wifi.connected() && state_setup_wifi == true && c_setup_wifi == true) {
          reconnect_wifi();
        }
        mqtt_wifi.loop();

      }
    }
  }
}

void f_Task_status_wifi_led( void * pvParameters )
{
  for (;;)
  {

    status_wifi = WiFi.status();

    if (wm.status_sta == true)
    {
      led_on();  delay(200);
      led_off(); delay(200);

    }

    else {

      //return status wifi
      //0,6  คือ เชื่อม wifi สำเร็จ แต่ออกเน็ตไม่ได้
      //1,5  คือ หา ap wifi ไม่เจอ หรือ หา router ไม่ได้
      //3    คือ เชื่อมต่อเน็ตสำเร็จ ออกเน็ตได้

      if (status_wifi == 0 || status_wifi == 6)
      {
        led_on();   delay(2000);
        led_off();  delay(1000);
      }
      else if (status_wifi == 1 || status_wifi == 5)
      {
        led_on();   delay(1000);
        led_off();  delay(1000);
      }

      else if (status_wifi == 3)
      {
        led_on();
      }
    }
  }
}


void f_Task_debug_other( void * pvParameters ) {
  debug("Task_debug_other running on core ");
  debugln(String(xPortGetCoreID()));


  for (;;) {
    f_debug_other();
  }
}



void f_Task_sensor( void * pvParameters ) {
  debug("Task_get_sensor running on core ");
  debugln(String(xPortGetCoreID()));


  for (;;) {
    f_get_sensor();
    delay(1000);
  }
}


void f_Task_serial( void * pvParameters ) {
  debug("Task_serial running on core ");
  debugln(String(xPortGetCoreID()));

  for (;;) {
    serialEvent();
    if (stringComplete) {
      f_cmd(inputString);
      inputString = "";
      stringComplete = false;
    }

  }
}



void f_Task_spiff( void * pvParameters ) {
  debug("Task_spiff running on core ");
  debugln(String(xPortGetCoreID()));


  for (;;) {

    if (flag_spiff == true)
    {

      if (code_cmd_spiff == 1) {
        Serial.println("putData");
        putData();

      }

      else if (code_cmd_spiff == 2) {
        //Last input data
        Serial.println("Last Data");
        lastData();
      }

      else if (code_cmd_spiff == 3) {
        Serial.println("shiftData");
        getTime_str();
        shiftData();
        //delay(1000);
      }

      else if (code_cmd_spiff == 4) {
        //Last shif data
        Serial.println("========== Last shif data  ==============");
        String sdFile = spiff.readFile(SPIFFS, "/writeShift.txt");
        Serial.println("readFile writeShift  : "); Serial.println(sdFile);
        Serial.println("************** Last shif data *******\n");
      }

      else if (code_cmd_spiff == 5) {
        Serial.println("unshiftData");
        unshiftData();
      }


      else if (code_cmd_spiff == 61) {
        //Read buffer
        int _cnt_payload = min_file_1;
        EEPROM.writeString(add_cnt_payload, String(_cnt_payload));
        EEPROM.commit();
        Serial.println("========== Read buffer  ==============");
        String sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
        delay(1000);
        Serial.println("1 readFile buffer  : "); Serial.println(sdFile);
        cnt_payload = min_file_1;
      }


      else if (code_cmd_spiff == 62) {
        //Read buffer
        int _cnt_payload = min_file_2;
        EEPROM.writeString(add_cnt_payload, String(_cnt_payload));
        EEPROM.commit();
        Serial.println("========== Read buffer  ==============");
        String sdFile = spiff.readFile(SPIFFS, "/buffer_2.txt");
        Serial.println("2 readFile buffer  : "); Serial.println(sdFile);
        cnt_payload = min_file_2;
      }


      else if (code_cmd_spiff == 63) {
        //Read buffer
        int _cnt_payload = min_file_3;
        EEPROM.writeString(add_cnt_payload, String(_cnt_payload));
        EEPROM.commit();
        Serial.println("========== Read buffer  ==============");
        String sdFile = spiff.readFile(SPIFFS, "/buffer_3.txt");
        Serial.println("3 readFile buffer  : "); Serial.println(sdFile);
        cnt_payload = min_file_3;
      }


      
      else if (code_cmd_spiff == 7) {
        //delete all data

        spiff.deleteFile(SPIFFS, "/buffer.txt");
        spiff.deleteFile(SPIFFS, "/buffer_2.txt");
        spiff.deleteFile(SPIFFS, "/buffer_3.txt"); 
        spiff.deleteFile(SPIFFS, "/writeShift.txt");
        spiff.deleteFile(SPIFFS, "/lastData.txt");

        byte cnt_payload = 0;
        EEPROM.writeString(add_cnt_payload, String(cnt_payload));
        EEPROM.commit();

        Serial.println("delete success");
      }


      else if (code_cmd_spiff == 8) {
        Serial.println("set_payload");
        set_payload();
      }

      else if (code_cmd_spiff == 9) {
        Serial.println("test_larg_payload");
        test_larg_payload();
      }


    }
    code_cmd_spiff = 0;
    flag_spiff = false;
    delay(200);
  }
}




void led_on() {
  digitalWrite(LED_PIN, HIGH);
}

void led_off() {
  digitalWrite(LED_PIN, LOW);
}
