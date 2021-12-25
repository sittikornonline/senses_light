#include "header.h"

void setup() {
  Serial.begin(115200);

  pinMode(batt_pin, INPUT);

  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  digitalWrite(red_pin, HIGH);
  digitalWrite(green_pin, HIGH);
  digitalWrite(blue_pin, HIGH);

  Serial.println("Hello");

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Ready!!!");


  //---------------- Deep Sleep -----------------------//

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  Serial.print("Batt : ");  Serial.println(digitalRead(batt_pin));

  String _pwrnum =  EEPROM.readString(0);

  pwrnum = _pwrnum.toInt();
  pwrnum++;
  _pwrnum = String(pwrnum);
  EEPROM.writeString(0, _pwrnum );
  EEPROM.commit();


  _pwrnum =  EEPROM.readString(0);
  putData( pwrnum );



  //  Serial.flush();
  //  esp_deep_sleep_start();
  //  Serial.println("This message will never be printed");




}

void loop()
{
  serialEvent();
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == 0x0A) {
      cmd();
    }
    else {
      inputString += inChar;
    }
  }
}

void cmd() {
  Serial.print("inputString : "); Serial.println(inputString);

  if (inputString == "0") {
    Serial.println("listDir");
    //listDir(SPIFFS, "/", 0);
  }

  else if (inputString == "1") {
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

    String str = "0";
    EEPROM.writeString(0, str);
    EEPROM.commit();
  }


  else if (inputString == "8") {
    ESP.restart();
  }


  // clear the string:
  inputString = "";
  stringComplete = false;

}
