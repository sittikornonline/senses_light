void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 0x0A) {
      stringComplete = true;
    }
    else {
      // add it to the inputString:
      inputString += inChar;
    }
  }
}



void f_cmd(String data)
{
  debug("data = "); debugln(data);
  if (data == "do1")
  {
    debug_other = true;
  }

  else if (data == "do0")
  {
    debug_other = false;
  }

  else if (data == "da1")
  {
    debug_all = true;
  }

  else if (data == "da0")
  {
    debug_all = false;
  }

  else if (data == "ds1")
  {
    debug_sensor = true;
  }

  else if (data == "ds0")
  {
    debug_sensor = false;
  }


  else if (data == "1") {
    //put data
    flag_spiff = true;
    code_cmd_spiff = 1;
  }

  else if (data == "2") {
    //last data
    flag_spiff = true;
    code_cmd_spiff = 2;
  }

  else if (data == "3") {
    //Serial.println("shiftData");
    flag_spiff = true;
    code_cmd_spiff = 3;
  }

  else if (data == "4") {
    //Last shif data
    Serial.println("\n\n========== Last shif data  ==============");
    flag_spiff = true;
    code_cmd_spiff = 4;
  }

  else if (data == "5") {
    //Serial.println("unshiftData");
    flag_spiff = true;
    code_cmd_spiff = 5;
  }


  else if (data == "6") {
    //Read buffer
    //Serial.println("\n\n========== Read buffer  ==============");
    flag_spiff = true;
    code_cmd_spiff = 6;
  }


  else if (data == "7") {
    //delete all data
    //Serial.println("\n\n========== delete all data  ==============");
    flag_spiff = true;
    code_cmd_spiff = 7;
  }

  else if (data == "8")
  {
    flag_spiff = true;
    code_cmd_spiff = 8;
  }

  else if (data == "9")
  { 
    code_cmd_spiff = 9;
    flag_spiff = true;
  }

  else if (data == "rs") {
    ESP.restart();
  }

  else if (data == "p1") {
    flag_putdata = true;
  }

  else if (data == "p0") {
    flag_putdata = false;
  }

  else if (data == "w1") {
    c_setup_wifi = true;
  }

  else if (data == "w0") {
    c_setup_wifi = false;
  }


  else if (data == "t1") {
    flag_upload_when_night = true;
  }

  else if (data == "t0") {
    flag_upload_when_night = false;
  }


  data = "";
  Serial.flush();
}




void debug(String str)
{
  if (debug_all == true)
  {
    Serial.print(str);
  }
}

void debugln(String str)
{
  if (debug_all == true)
  {
    Serial.println(str);
  }
}


void f_debug_other()
{
  if (debug_other == true)
  {
    unsigned int now_other = millis();
    if (now_other - lastMsg_other > interval_other) {
      lastMsg_other = now_other;

      //      debug("status_sta  : "); debugln(String(wm.status_sta));
      //      debug("status_wifi : "); debugln(String(status_wifi));
      //      debug("FreeHeap    : "); debugln(String(ESP.getFreeHeap()));

      Serial.print("mac_address       : "); Serial.println(mac_address);
      Serial.print("flag_upload_when_night        : "); Serial.println(flag_upload_when_night);
      Serial.print("c_setup_wifi      : "); Serial.println(c_setup_wifi);
      Serial.print("status_mqtt       : "); Serial.println(status_mqtt);
      Serial.print("emptyFile         : "); Serial.println(emptyFile);
      Serial.print("flag_putdata      : "); Serial.println(flag_putdata);
      Serial.println("----------------------------------------");
      debugln("---------------------");
    }
  }
}
