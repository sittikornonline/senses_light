void f_Task_ble_connect( void * pvParameters ) {

  Serial.print("f_Task_ble_connect running on core ");
  Serial.println(String(xPortGetCoreID()));

  for (;;) {
    pCharacteristic->setValue(set_payload_connect().c_str());
    pCharacteristic->notify();
  }
}



void f_Task_ble_non_connect( void * pvParameters ) {

  Serial.print("f_Task_ble_non_connect running on core ");
  Serial.println(String(xPortGetCoreID()));

  for (;;) {


    //Advers 1
    setBeacon();
    pAdvertising->start();
    delay(1000 * time_advers);

    //Advers 2
    setBeacon();
    delay(1000 * time_advers);

    //Advers 3
    setBeacon();
    delay(1000 * time_advers);

    gettimeofday(&nowTimeStruct, NULL);
    //Serial.printf("start ESP32 %d\n", bootcount++);
    Serial.println("\ndeep sleep");
    Serial.println("------------------ END ----------------------");

    last = nowTimeStruct.tv_sec;
    lastTenth = nowTimeStruct.tv_sec * 10; // Time since last reset as 0.1 second resolution counter
    esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);

  }
}
