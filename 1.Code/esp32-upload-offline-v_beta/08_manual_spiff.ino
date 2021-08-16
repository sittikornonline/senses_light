void putData(int _brewtime)
{
  if (_brewtime >= 0 && _brewtime <= 2)
  {
  }
  else {
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
      Serial.println("SPIFFS Mount Failed");
      return;
    }

    pwrnum;
    brewnum++;
    brewtime = _brewtime;
    brewcnt++;

    Serial.printf("pwrnum       : %d\n", pwrnum);
    Serial.printf("brewnum      : %d\n", brewnum);
    Serial.printf("brewtime     : %d\n", brewtime);

    String dataFileName = String(pwrnum) + "," + String(brewnum) + "," + String(brewtime) + String(ends);

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

    Serial.println("************** putData Success *******\n");
    emptyFile = false;
  }
}



void shiftData()
{

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

    char tChar[sizeBuffer];
    sdFile.toCharArray(tChar, sizeBuffer);
    for (int i = 0; i <= sdFileLength;  i++)
    {
      lencutfist ++;
      if (tChar[i] == '\n') {
        break;
      }
      else {
        shiftString += tChar[i];
      }
    }

    _shiftString = shiftString;

    Serial.print("_shiftString : "); Serial.println(_shiftString);

    StringSplitter *splitter = new StringSplitter(_shiftString, ',', no_of_data);  // new StringSplitter(string_to_split, delimiter, limit)
    int itemCount = splitter->getItemCount();
    //Serial.print("Item count: "); Serial.println(itemCount);

    int k , data[no_of_data];
    for (k = 0; k <= itemCount; k++) {
      String item = splitter->getItemAtIndex(k);
      //Serial.print("Item @ index "); Serial.print(k); Serial.print(" : "); Serial.println(item);
      data[k] = item.toInt();
    }


    //------ save data to writeshift.txt first  -> if need unshift give read from writeshift.txt-> spiff (append)
    String _pwrnum     =  String(data[0]);
    String _brewnum    =  String(data[1]);
    String _brewtime   =  String(data[2]);

    String dataFileName =  _pwrnum + "," + _brewnum + "," + _brewtime +   String(ends);
    //Serial.print("--> dataFileName  : "); Serial.println(dataFileName);

    writeShift(dataFileName);



    Serial.printf(" %d , %d , %d, %d \n", data[0], data[1], data[2], data[3]); 
 

    for (int i = lencutfist; i < sdFileLength;  i++)
    {
      cutFirstString += tChar[i];
    }

    spiff.deleteFile(SPIFFS, "/buffer.txt");
    //Serial.println("deleteFile");

    _cutFirstString = cutFirstString;
    //Serial.print("_cutFirstString: "); Serial.println(_cutFirstString);

    int lengths = _cutFirstString.length() + 1;
    char dataFileName1[lengths];
    _cutFirstString.toCharArray(dataFileName1, lengths);
    spiff.appendFile(SPIFFS, "/buffer.txt", dataFileName1);

    //Serial.println("========== readFile ==============");
    sdFile = spiff.readFile(SPIFFS, "/buffer.txt");
    Serial.println("readFile  : "); Serial.println(sdFile);
  }

  Serial.println("************** shiftData Success *******\n\n");

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

    StringSplitter *splitter = new StringSplitter(_shiftString, ',', no_of_data);  // new StringSplitter(string_to_split, delimiter, limit)
    int itemCount = splitter->getItemCount();
    //Serial.print("Item count: "); Serial.println(itemCount);

    int k , data[no_of_data];
    for (k = 0; k < itemCount; k++) {
      String item = splitter->getItemAtIndex(k);
      //Serial.print("Item @ index "); Serial.print(k); Serial.print(" : "); Serial.println(item);
      data[k] = item.toInt();
    }

    Serial.printf("shiftData  pwrnum    : %d\n",  data[0]);
    Serial.printf("shiftData  brewnum   : %d\n",  data[1]);
    Serial.printf("shiftData  brewtime  : %d\n",  data[2]);

  }

  //Serial.println("************** writeShift Success *******\n\n");

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


  Serial.println("************** unshiftData Success *******\n\n");

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

    StringSplitter *splitter = new StringSplitter(_shiftString, ',', no_of_data);  // new StringSplitter(string_to_split, delimiter, limit)
    int itemCount = splitter->getItemCount();
    //Serial.print("Item count: "); Serial.println(itemCount);

    int k , data[no_of_data];
    for (k = 0; k < itemCount; k++) {
      String item = splitter->getItemAtIndex(k);
      //Serial.print("Item @ index "); Serial.print(k); Serial.print(" : "); Serial.println(item);
      data[k] = item.toInt();
    }


    pwrnum   =  String(data[0]).toInt();
    brewnum  =  String(data[1]).toInt();
    brewtime =  String(data[2]).toInt();

    Serial.print("last pwrnum   : "); Serial.println(pwrnum);
    Serial.print("last brewnum  : "); Serial.println(brewnum);
    Serial.print("last brewtime : "); Serial.println(brewtime);

    Serial.println("************** lastData Success *******\n\n");


  }
}
