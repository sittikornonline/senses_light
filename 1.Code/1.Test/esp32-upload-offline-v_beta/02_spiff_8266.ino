//#if esp8266 
//
///* PULL these function from PIC Firmware : 23Mar2021, version 8.0 */
//String listDir(const char * dirname) {
//  String _readDir = "lsfile:";
//
//  Dir root = LittleFS.openDir(dirname);
//
//  while (root.next()) {
//    File file = root.openFile("r");
//    _readDir += root.fileName();
//    _readDir += "=";
//    _readDir += String(root.fileSize());
//    _readDir += "|";
//    file.close();
//  }
//  return _readDir;
//}
//
//bool writeFile(const char * path, const char * message) {
//
//  File file = LittleFS.open(path, "w");
//  if (!file) {
//    return false;
//  }
//  if (file.print(message)) {
//    file.close();
//    return true;
//  } else {
//    file.close();
//    return false;
//  }
//}
//
//bool write_EOL_File(const char * path, const char * message) {
//
//  File file = LittleFS.open(path, "a");
//  if (!file) {
//    return false;
//  }
//  unsigned long limitsize = txtsizeperfile - offset_write_EOL;
//  if (file.size() > limitsize) {
//    Serial.println("Full size of offline file.");
//    return false;
//  }
//  if (file.print(message)) {
//    file.close();
//    return true;
//  } else {
//    file.close();
//    return false;
//  }
//}
//
//String readFile(const char * path) {
//  String _readStr = "";
//
//  File file = LittleFS.open(path, "r");
//  if (!file) {
//    return "";
//  }
//
//  while (file.available()) {
//    _readStr += char(file.read());
//  }
//
//  file.close();
//  return _readStr;
//}
//
//bool deleteFile(const char * path) {
//  if (LittleFS.remove(path)) {
//    return true;
//  } else {
//    return false;
//  }
//}
//
//
//
//
//void dumpFile(String _filename) {
//
//  /* -- Upload Offline file to server -- */
//  size_t _uploadFileSize;
//  File uploadFile = LittleFS.open(_filename.c_str(), "r");
//
//  _uploadFileSize = uploadFile.size();
//  Serial.println("Start Upload offline file len = " + String(_uploadFileSize) + " bytes.");
//
//  /* -- Check size of dump file -- */
//  if (_uploadFileSize >= txtsizeperfile) {
//
//    Serial.println("Cannot dump file because of size is over");
//    pushmessage("file size is over " + String(txtsizeperfile) + " bytes.");
//
//  } else {
//
//    unsigned long _cnt = 0;
//
//    while (uploadFile.available()) {
//      text[_cnt] = char(uploadFile.read());
//      _cnt++;
//    }
//
//    uploadFile.close();
//
//    Serial.println("text size = " + String(_cnt));
//
//    String bodyTxt = body("wfmcaddr", WiFi.macAddress().c_str());
//    uploadUnixTime = getTime();
//
//    String uploadFilename = WiFi.macAddress() + "_" + String(uploadUnixTime) + ".csv";
//    uploadFilename.replace(":", "");
//
//    String bodyFile = body("file", uploadFilename.c_str());
//    String bodyEnd =  String("--") + BOUNDARY + String("--\r\n");
//
//    size_t allLen = bodyTxt.length() + bodyFile.length() + _cnt + bodyEnd.length();
//    String headerTxt =  set_header(allLen);
//
//    WiFiClient client;
//    Serial.println("WiFiClient processing..");
//
//    if (!client.connect(UPLOAD_HOST, UPLOAD_PORT)) {
//      Serial.println("connection failed");
//    } else {
//
//      Serial.println("host connected !");
//
//      client.print(headerTxt + bodyTxt + bodyFile);
//      client.write(text, _cnt);
//      client.print("\r\n" + bodyEnd);
//
//      delay(20);
//      String serverRes = "";
//      long tOut = millis() + TIMEOUT;
//      while (client.connected() && tOut > millis()) {
//        if (client.available()) {
//          serverRes = client.readStringUntil('\r');
//        } else {
//          //..
//        }
//      }
//      Serial.println("serverRes = " + serverRes);
//
//    }
//  }
//
//}
//
//#endif
// 
