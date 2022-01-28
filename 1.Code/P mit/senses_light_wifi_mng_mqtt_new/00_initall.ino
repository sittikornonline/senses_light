void initall()
{
  Serial.begin(115200);
  Serial.flush();

  f_get_sensor(); delay(1000);
  Serial.println("get sensor ok");



  //------------------------ Serial CMD ----------------------//
  inputString.reserve(200);
  payload_w.reserve(2000);
  payload_mqtt.reserve(90000);


  //------------------------ EEPROM ----------------------//

  if (!EEPROM.begin(EEPROM_SIZE)) {
    debugln("Failed to initialise EEPROM");
    debugln("Restarting...");
    delay(1000);
  }




  //-------------- WiFiManager -------------------//
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);


  led_off();

  if (wifi_mng == true)
  {
    String _ssid = EEPROM.readString(add_ssid);
    String _passw = EEPROM.readString(add_passw);


    ssid[sizeof(_ssid)];
    _ssid.toCharArray(ssid, sizeof(ssid));


    passw[sizeof(_passw)];
    _passw.toCharArray(passw, sizeof(passw));

  }


  xTaskCreatePinnedToCore(
    f_Task_reconnect_wifi,   /* Task function. */
    "Task_reconnect_wifi",     /* name of task. */
    30000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_reconnect_wifi,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


  xTaskCreatePinnedToCore(
    f_Task_status_wifi_led,   /* Task function. */
    "Task_status_wifi_led",     /* name of task. */
    2000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_status_wifi_led,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


  xTaskCreatePinnedToCore(
    f_Task_debug_other,   /* Task function. */
    "Task_debug_other",     /* name of task. */
    3000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_debug_other,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


  xTaskCreatePinnedToCore(
    f_Task_serial,   /* Task function. */
    "Task_serial",     /* name of task. */
    5000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_serial,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */

  xTaskCreatePinnedToCore(
    f_Task_spiff,   /* Task function. */
    "Task_spiff",     /* name of task. */
    13000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_spiff,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */



  xTaskCreatePinnedToCore(
    f_Task_sensor,   /* Task function. */
    "Task_sensor",     /* name of task. */
    15000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_sensor,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


  mac_address = String(WiFi.macAddress());
  mac_address.replace(":", "");
  Serial.print("mac_address : "); Serial.println(mac_address);

  mqtt_pub = mqtt_pub + mac_address;
  mqtt_sub = mqtt_sub + mac_address;

  mqtt_wifi.setServer(mqtt_broker, mqtt_port);
  mqtt_wifi.setCallback(mqttCallback);

 

}
