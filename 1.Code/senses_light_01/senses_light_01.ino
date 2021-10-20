#include "header.h"


void setup() {
  f_init_all();

}

void loop() {

  // Create the BLE Device
  BLEDevice::init(gateway_id.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_N12);

  pAdvertising = BLEDevice::getAdvertising();
  setBeacon();
  pAdvertising->start();
  delay(1000*time_advert);


  gettimeofday(&nowTimeStruct, NULL);
  Serial.printf("start ESP32 %d\n", bootcount++);
  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", nowTimeStruct.tv_sec, nowTimeStruct.tv_sec - last);

  last = nowTimeStruct.tv_sec;
  lastTenth = nowTimeStruct.tv_sec * 10; // Time since last reset as 0.1 second resolution counter
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);


  //  ESP.restart();
}
