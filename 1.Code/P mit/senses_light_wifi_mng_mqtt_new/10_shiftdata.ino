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


  if (cnt_payload >= min_file_1 && emptyFile == false)
  {

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
    Serial.println("readFile  : "); Serial.println(sdFile);

    unsigned int sdFileLength = sdFile.length();


    if (sdFile == "") {
      emptyFile = true;
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
        spiff.deleteFile(SPIFFS, "/buffer.txt");
        //Serial.println("deleteFile");
      }

      //Serial.println("========== readFile ==============");
      sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
      Serial.println("1 readFile  : "); Serial.println(sdFile);

      delay(1000); // do not remove
    }
  }

  else if (cnt_payload >= min_file_2 && emptyFile_2 == false)
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


  else if (cnt_payload >= min_file_3 && emptyFile_3 == false)
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


 

  if (emptyFile == true && emptyFile_2 == true && emptyFile_3 == true)
  {
    byte cnt_payload = 0;
    EEPROM.writeString(add_cnt_payload, String(cnt_payload));
    EEPROM.commit();
  }


}
