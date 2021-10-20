 void cmd_serial()
{
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    cmd_str(inputString);
    inputString = "";
    stringComplete = false;
  }
}


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

void cmd_str(String inputString)
{
  Serial.print("inputString : "); Serial.println(inputString);

  if (inputString == "1") {
    Serial.println("putData");
    cnt++;
    putData( cnt );
  }

  else if (inputString == "2") {
    //Last input data
    lastData();
  }

  else if (inputString == "3") {
    Serial.println("shiftData");
    shiftData();
  }

  else if (inputString == "4") {
    //Last shif data
    Serial.println("\n\n========== Last shif data  ==============");
    String sdFile = spiff.readFile(SPIFFS, "/writeShift.txt");
    Serial.println("readFile writeShift  : "); Serial.println(sdFile);
    Serial.println("************** Last shif data *******\n");
  }

  else if (inputString == "5") {
    Serial.println("unshiftData");
    unshiftData();
  }


  else if (inputString == "6") {
    //Read buffer
    Serial.println("\n\n========== Read buffer  ==============");
    String sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
    Serial.println("readFile buffer  : "); Serial.println(sdFile);
    Serial.println("************** Read buffer *******\n");
  }



  else if (inputString == "7") {
    //delete all data

    spiff.deleteFile(SPIFFS, "/buffer.txt");
    spiff.deleteFile(SPIFFS, "/writeShift.txt");
    spiff.deleteFile(SPIFFS, "/lastData.txt");
    spiff.deleteFile(SPIFFS, "/opencap.txt");
    Serial.println("deleteFile");
    pwrnum = 0;
    brewnum = 0;
    brewtime = 0;
  }
  // clear the string:
  inputString = "";
  stringComplete = false;
}
