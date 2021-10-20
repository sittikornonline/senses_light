void run_work()
{



  /* EVENT 8 : Offline - Record log */
  unsigned int now = millis();
  if (now - lastMsg_dump > interval_dump) {
    lastMsg_dump = now;
    //.. do something about log txt record..
    if (WiFi.status() == WL_CONNECTED) 
    { 
      // -- ONLINE --
      epochTime = getTime();
      Serial.println("ONLINE / epochTime = " + String(epochTime));

    }
    else
    {
      cnt++;
      putData( cnt );
    }

    //Serial.flush();

  }
}



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
