unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}



String set_header(size_t length) {

  String  data;

  data =  F("POST /offlinelog HTTP/1.1\r\n");
  data += F("Content-Type: multipart/form-data; boundary=");
  data += BOUNDARY;
  data += "\r\n";
  data += F("Host: ");
  data += UPLOAD_HOST;
  data += F(":");
  data += UPLOAD_PORT;
  data += F("\r\n");
  data += F("accept-encoding: utf-8\r\n");
  data += F("Connection: keep-alive\r\n");
  data += F("content-length: ");
  data += String(length);
  data += "\r\n";
  data += "\r\n";

  Serial.println(data);

  return (data);
}

#if wifi

String body(String content , String message) {

  String data;
  data = "--";
  data += BOUNDARY;
  data += F("\r\n");
  if (content == "file")
  {
    data += "Content-Disposition: form-data; name=\"offlinelog\"; filename=\"" + message + "\"\r\n";
    data += "Content-Type: text/plain\r\n";
    data += "\r\n";
  }
  else
  {
    data += "Content-Disposition: form-data; name=\"" + content + "\"\r\n";
    data += "\r\n";
    data += message;
    data += "\r\n";
  }

  Serial.println(data);

  return (data);
}
#endif
