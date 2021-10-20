#include "header.h"

class MyServerCallbacks: public BLEServerCallbacks {

};


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      Serial.printf("Advertised Device: %s", advertisedDevice.toString().c_str());
      Serial.print(" RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
};

void setup() {
  f_init_all();

}

void loop() {


  pCharacteristic->setValue(set_payload().c_str());
  pCharacteristic->notify();
  delay(1000 * time_advers);


  gettimeofday(&nowTimeStruct, NULL);
  //Serial.printf("start ESP32 %d\n", bootcount++);
  Serial.println("\ndeep sleep");
  Serial.println("------------------ END ----------------------");

  last = nowTimeStruct.tv_sec;
  lastTenth = nowTimeStruct.tv_sec * 10; // Time since last reset as 0.1 second resolution counter
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);

}
