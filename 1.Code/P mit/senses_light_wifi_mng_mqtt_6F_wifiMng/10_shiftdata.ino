void shiftData()
{

  String _cnt_payload = EEPROM.readString(add_cnt_payload);
  int _cnt_payload_int = _cnt_payload.toInt();
  if (_cnt_payload_int <= 0 || _cnt_payload == "")
  {
    cnt_payload = 0;
  }
  cnt_payload = _cnt_payload_int;

  Serial.print("shiftData cnt_payload       : "); Serial.println(cnt_payload);



  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  Serial.println("\n\n========== shiftData  ==============");

  String  sdFile = "";
  String  shiftString = "";
  String  _shiftString = "";
  String  cutFirstString = "";
  String  _cutFirstString = "";
  unsigned int lencutfist = 0;


  if (cnt_payload >= min_file_1 && emptyFile_1 == false)
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_1.txt");
    Serial.println("readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "") {
      emptyFile_1 = true;
      Serial.println("1 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_1.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_1.txt");
      Serial.println("1 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }

  else if (cnt_payload >= min_file_2 && emptyFile_2 == false )
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_2.txt");
    Serial.println("2 readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "") {
      emptyFile_2 = true;
      Serial.println("2 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_2.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_2.txt");
      Serial.println("2 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }


  else if (cnt_payload >= min_file_3 && emptyFile_3 == false )
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_3.txt");
    Serial.println("3 readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "" ) {
      emptyFile_3 = true;
      Serial.println("3 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_3.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_3.txt");
      Serial.println("3 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }



  else if (cnt_payload >= min_file_4 && emptyFile_4 == false )
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_4.txt");
    Serial.println("4 readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "" ) {
      emptyFile_4 = true;
      Serial.println("4 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_4.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_4.txt");
      Serial.println("4 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }


  else if (cnt_payload >= min_file_5 && emptyFile_5 == false )
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_5.txt");
    Serial.println("5 readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "" ) {
      emptyFile_5 = true;
      Serial.println("3 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_5.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_5.txt");
      Serial.println("5 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }



  else if (cnt_payload >= min_file_6 && emptyFile_6 == false )
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer_6.txt");
    Serial.println("6 readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "" ) {
      emptyFile_6 = true;
      Serial.println("6 Empty file : shiftData");
    }
    else {

      payload_mqtt =  sdFile;

      int msgLng = setPayload_w().length();
      mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
      bool status_send = mqtt_wifi.print(setPayload_w().c_str());
      debug("Mode W - payload : "); debugln(setPayload_w());
      debug("status_send      : "); debugln(String(status_send));
      payload_mqtt = "";


      if (status_send == true)
      {
        spiff.deleteFile(SPIFFS, "/buffer_6.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer_6.txt");
      Serial.println("6 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }


  bool flag_remove_cnt_payload = false;

  if ((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_1) && emptyFile_1 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("A flag_remove_cnt_payload");
  }

  else if ((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_2) &&
           emptyFile_1 == true && emptyFile_2 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("B flag_remove_cnt_payload");
  }


  else if ((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_3) &&
           emptyFile_1 == true && emptyFile_2 == true && emptyFile_3 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("C flag_remove_cnt_payload");
  }

  else if ((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_4) &&
           emptyFile_1 == true && emptyFile_2 == true && emptyFile_3 == true && emptyFile_4 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("D flag_remove_cnt_payload");
  }

  else if ((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_5) &&
           emptyFile_1 == true && emptyFile_2 == true && emptyFile_3 == true && emptyFile_4 == true && emptyFile_5 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("E flag_remove_cnt_payload");
  }


  else if (((cnt_payload >= min_file_1 &&  cnt_payload <= max_file_6)  || cnt_payload > max_file_6) &&
           emptyFile_1 == true && emptyFile_2 == true && emptyFile_3 == true && emptyFile_4 == true && emptyFile_5 == true && emptyFile_6 == true)
  {
    flag_remove_cnt_payload = true;
    Serial.println("F flag_remove_cnt_payload");
  }


  if ( flag_remove_cnt_payload == true)
  {
    int msgLng = setPayload_send_finish().length();
    mqtt_wifi.beginPublish(mqtt_pub.c_str(), msgLng, false);
    bool status_send = mqtt_wifi.print(setPayload_send_finish().c_str());
    debug("Mode A - payload : "); debugln(setPayload_send_finish());
    debug("status_send      : "); debugln(String(status_send));
    delay(1000);

    byte cnt_payload = 0;
    EEPROM.writeString(add_cnt_payload, String(cnt_payload));
    EEPROM.commit();
    delay(1500);

    emptyFile_1 = true;
    emptyFile_2 = true;
    emptyFile_3 = true;
    emptyFile_4 = true;
    emptyFile_5 = true;
    emptyFile_6 = true;
  }


}
