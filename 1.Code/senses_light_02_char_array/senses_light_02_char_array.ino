#include "header.h"


void setup() {
  f_init_all();

}

void loop() {

  // Create the BLE Device
  BLEDevice::init(gateway_id.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_N12);

  pAdvertising = BLEDevice::getAdvertising();

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
