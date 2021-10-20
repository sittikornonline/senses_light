void initall()
{
 
  Serial.begin(115200);
  //timeClient.begin(); 

#if c_spiff
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
//  listDir(SPIFFS, "/", 0);
//  writeFile(SPIFFS, "/hello.txt", "Hello ");
#endif

 
#if dump
  xTaskCreatePinnedToCore(
    f_Task_dump,   /* Task function. */
    "Task_dump",     /* name of task. */
    35000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_dump,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */

#endif

#if serial
  xTaskCreatePinnedToCore(
    f_Task_serial,   /* Task function. */
    "Task_serial",     /* name of task. */
    15000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_serial,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */

#endif


#if mqtt

  xTaskCreatePinnedToCore(
    f_Task_Reconnect_wifi,   /* Task function. */
    "Task_Reconnect_wifi",     /* name of task. */
    27000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_Reconnect_wifi,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */

  // client.setBufferSize(2000);             /* Size of sending message in BYTE */
  // client.setKeepAlive(15);                /* in Sec., Default is 15 sec. */
  mqtt_wifi.setServer(mqtt_server, mqtt_port);    /* Connect to MQTT */
  mqtt_wifi.setCallback(mqttCallback);

#endif

#if esp8266
  delay(1000);
  Serial.println("Test OFFLINE record data into FS..");

  Serial.println();
  Serial.println("Initializing FileFS.begin()..");
  FileFS.begin();

  delay(1000);

  Serial.println();
  Serial.println("List dir..");
  Serial.println(listDir("/"));

  Serial.println();
  Serial.println("Check info FS..");


  FSInfo fs_info;
  FileFS.info(fs_info);
  Serial.println("totalBytes = " + String(fs_info.totalBytes) + " bytes.");
  Serial.println("usedBytes = " + String(fs_info.usedBytes) + " bytes.");
  Serial.println("blockSize = " + String(fs_info.blockSize) + " bytes.");
  Serial.println("pageSize = " + String(fs_info.pageSize) + " bytes.");
  Serial.println("maxOpenFiles = " + String(fs_info.maxOpenFiles) + " bytes.");
  Serial.println("maxPathLength = " + String(fs_info.maxPathLength) + " bytes.");
  delay(1000);

  Serial.println();
  Serial.println("RE-List dir..");
  Serial.println(listDir("/"));

  setup_wifi();
#endif

 

state_initall = true;
}
