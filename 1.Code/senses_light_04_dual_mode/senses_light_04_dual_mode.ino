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


  

  //----------------- Non connect
  // Create the BLE Device
  BLEDevice::init(gateway_id.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_N12);
  pAdvertising = BLEDevice::getAdvertising();

}

void loop() {
 
}
