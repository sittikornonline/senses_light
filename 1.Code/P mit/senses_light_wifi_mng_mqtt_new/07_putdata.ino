void putData()
{

  set_payload(); delay(1000);

  String max_log = "";
  if (test_max_log == true)
  {
    max_log = test_larg_payload();
  }


  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  String dataFileName = "";
  dataFileName.reserve(20000);

  if (test_max_log == true)
  {
    //dataFileName =  max_log + ends;
    dataFileName = payload_str  + ends;
  }
  else {
    dataFileName = payload_str  + ends;
  }

  unsigned int lengths = dataFileName.length() + 1;
  char dataFileName1[lengths];

  if (cnt_payload >= min_file_1 && cnt_payload <= max_file_1)
  {

    dataFileName.toCharArray(dataFileName1, lengths);
    spiff.appendFile(SPIFFS, "/buffer.txt", dataFileName1);
    Serial.print("buffer appendFile => ");   Serial.println(dataFileName1);

    //Serial.println("========== readFile ==============");

    String sdFile = "";
    sdFile.reserve(45000);

    sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
    Serial.println("buffer readFile  : "); Serial.println(sdFile);
    emptyFile = false;
    delay(1500);

  }

  else if (cnt_payload >= min_file_2 && cnt_payload <= max_file_2)
  {

    dataFileName.toCharArray(dataFileName1, lengths);
    spiff.appendFile(SPIFFS, "/buffer_2.txt", dataFileName1);
    Serial.print("buffer_2 appendFile => ");   Serial.println(dataFileName1);

    //Serial.println("========== readFile ==============");

    String sdFile = "";
    sdFile.reserve(45000);

    sdFile = spiff.readFile(SPIFFS, "/buffer_2.txt");
    Serial.println("buffer_2 readFile  : "); Serial.println(sdFile);
    emptyFile_2 = false;
    delay(1500);

  }

  else if (cnt_payload >= min_file_3 && cnt_payload <= max_file_3)
  {

    dataFileName.toCharArray(dataFileName1, lengths);
    spiff.appendFile(SPIFFS, "/buffer_3.txt", dataFileName1);
    Serial.print("buffer_3 appendFile => ");   Serial.println(dataFileName1);

    //Serial.println("========== readFile ==============");

    String sdFile = "";
    sdFile.reserve(45000);

    sdFile = spiff.readFile(SPIFFS, "/buffer_3.txt");
    Serial.println("buffer_3 readFile  : "); Serial.println(sdFile);
    emptyFile_3 = false;
    delay(1500);

  }



  payload_str = "";
  flag_putdata = false;

  //Go to sleep now
  if (test_max_log == false)
  {
    delay(2000);
    esp_deep_sleep_start();
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
