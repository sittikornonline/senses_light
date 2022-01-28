void putData()
{

  set_payload(); delay(1000);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  String dataFileName = payload_str + ends;

  int lengths = dataFileName.length() + 1;
  char dataFileName1[lengths];

  dataFileName.toCharArray(dataFileName1, lengths);
  spiff.appendFile(SPIFFS, "/buffer.txt", dataFileName1);
  Serial.print("appendFile => ");   Serial.println(dataFileName1);

  //Serial.println("========== readFile ==============");
  String sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
  Serial.println("readFile  : "); Serial.println(sdFile);

  // Save lastData for open machine count continut...
  spiff.writeFile(SPIFFS, "/lastData.txt", dataFileName1);

  emptyFile = false;
  payload_str = "";
  flag_putdata = false;

  //Go to sleep now
  esp_deep_sleep_start();

}



void shiftData()
{

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

  //Serial.println("========== readFile ==============");
  sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
  Serial.println("readFile  : "); Serial.println(sdFile);

  unsigned int sdFileLength = sdFile.length();


  if (sdFile == "") {
    emptyFile = true;
    Serial.println("Empty file : shiftData");
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
    Serial.println("readFile  : "); Serial.println(sdFile);

    delay(1000); // do not remove
  }


}


void writeShift(String dataFileName) { // Write firt value to file : if need unshift give read from writeShift

  //Serial.println("\n\n========== writeShift  ==============");

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  //Serial.println("========== writeFile ==============");
  int lengths = dataFileName.length() + 1;
  char dataFileName1[lengths];

  dataFileName.toCharArray(dataFileName1, lengths);
  spiff.writeFile(SPIFFS, "/writeShift.txt", dataFileName1);


  //Serial.println("========== readFile ==============");
  String sdFile = spiff.readFile(SPIFFS, "/writeShift.txt");
  //Serial.println("readFile  : "); Serial.println(sdFile);

  unsigned int sdFileLength = sdFile.length();
  String shiftString = "";
  String _shiftString = "";


  if (sdFile == "") {
    emptyFile = true;
    Serial.println("Empty file : shiftData");
  }
}


void unshiftData()
{
  Serial.println("\n\n========== unshiftData  ==============");

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  //Serial.println("========== readFile ==============");
  String sdFile_writeShift = spiff.readFile(SPIFFS, "/writeShift.txt");
  //Serial.println("sdFile_writeShift  : "); Serial.println(sdFile_writeShift);


  String sdFile_buffer = spiff.readFile(SPIFFS, "/buffer.txt");
  Serial.println("readFile sdFile_buffer  : "); Serial.println(sdFile_buffer);


  spiff.deleteFile(SPIFFS, "/buffer.txt");
  Serial.println("deleteFile");

  String dataFileName =  sdFile_writeShift + sdFile_buffer;

  int lengths = dataFileName.length() + 1;
  char dataFileName1[lengths];

  dataFileName.toCharArray(dataFileName1, lengths);
  spiff.appendFile(SPIFFS, "/buffer.txt", dataFileName1);
  Serial.print("appendFile: ");   Serial.println(dataFileName1);

  //Serial.println("========== readFile ==============");
  sdFile_buffer = spiff.readFile(SPIFFS, "/buffer.txt");
  Serial.println("readFile sdFile_buffer  : "); Serial.println(sdFile_buffer);

  spiff.deleteFile(SPIFFS, "/writeShift.txt");
  //Serial.println("deleteFile");



}


void lastData() {

  Serial.println("\n\n========== lastData  ==============");


  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  //Serial.println("========== readFile ==============");

  String sdFile = spiff.readFile(SPIFFS, "/lastData.txt");
  Serial.println("readFile  : "); Serial.println(sdFile);

  unsigned int sdFileLength = sdFile.length();
  String shiftString = "";
  String _shiftString = "";

  if (sdFile == "") {
    emptyFile = true;
    Serial.println("Empty file : lastData");
  }
  else {

    char tChar[sizeBuffer];
    sdFile.toCharArray(tChar, sizeBuffer);
    for (int i = 0; i <= sdFileLength;  i++)
    {

      if (tChar[i] == '\n') {
        break;
      }
      else {
        shiftString += tChar[i];
      }
    }

    _shiftString = shiftString;

    Serial.print("_shiftString : "); Serial.println(_shiftString);




  }
}
