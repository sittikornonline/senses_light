#if esp32

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return ;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}


bool deleteFile(const char * path) {
  if (deleteFile(path)) {
    return true;
  } else {
    return false;
  }
}


void dumpFile(String _filename) {

  /* -- Upload Offline file to server -- */

  String uploadFile = spiff.readFile(SPIFFS, _filename.c_str());
  Serial.println("dumpFile Read file : ");
  Serial.println(uploadFile);

  //size_t _uploadFileSize = uploadFile.size;
  int _uploadFileSize = uploadFile.length();
  Serial.println("Start Upload offline file len = " + String(_uploadFileSize) + " bytes.");

  /* -- Check size of dump file -- */
  if (_uploadFileSize >= txtsizeperfile) {

    Serial.println("Cannot dump file because of size is over");
#if mqtt
    pushmessage("file size is over " + String(txtsizeperfile) + " bytes.");
#endif

  } else {

    unsigned long _cnt = 0;
    _cnt = uploadFile.length();
    text[_cnt];
    uploadFile.toCharArray(text, _cnt);

    Serial.println("text size = " + String(_cnt));

    String bodyTxt = body("wfmcaddr", WiFi.macAddress().c_str());
    uploadUnixTime = getTime();

    //String uploadFilename = WiFi.macAddress() + "_" + String(uploadUnixTime) + ".csv";

    //String uploadFilename = WiFi.macAddress() + "_buffer.txt";
    String uploadFilename = "/buffer.txt";

    uploadFilename.replace(":", "");

    String bodyFile = body("file", uploadFilename.c_str());
    String bodyEnd =  String("--") + BOUNDARY + String("--\r\n");

    int allLen = bodyTxt.length() + bodyFile.length() + _cnt + bodyEnd.length();
    String headerTxt =  set_header(allLen);

    WiFiClient client;
    Serial.println("WiFiClient processing..");

    if (!client.connect(UPLOAD_HOST, UPLOAD_PORT)) {
      Serial.println("connection failed");
    } else {

      Serial.println("host connected !");

      client.write(text, _cnt);
      client.print("\r\n" + bodyEnd);

      delay(20);
      String serverRes = "";
      long tOut = millis() + TIMEOUT;
      while (client.connected() && tOut > millis()) {
        if (client.available()) {
          serverRes = client.readStringUntil('\r');
        } else {
          //..
        }
      }
      Serial.println("serverRes = " + serverRes);

    }
  }

}


void list_file()
{
  if (state_list_file == true)
  {
    //
    //    File root = SPIFFS.open("/");
    //    File file = root.openNextFile();
    //    String str = "";
    //    while (file) {
    //      Serial.print("FILE: ");
    //      Serial.println(file.name());
    //      str += String(file.name());
    //      file = root.openNextFile();
    //    }
    //    if (status_mqtt == true)
    //    {
    //      pushmessage(str);
    //    }
    //    state_list_file = false;
    //  }
    listDir(SPIFFS, "/", 0);
    state_list_file = false;
  }
}

#endif
