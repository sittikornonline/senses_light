#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BLEBeacon.h"
#include "BLEAdvertising.h"
#include "BLEEddystoneURL.h"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEAdvertising *pAdvertising;

String mac_address_wifi = "";
String device_id = "";

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


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
  Serial.begin(115200);

  // Create the BLE Device


  mac_address_wifi = WiFi.macAddress();
  mac_address_wifi.replace(":", "");
  device_id = "SEN_" + mac_address_wifi.substring(9, 12);

  Serial.print("MAC Address:  "); Serial.println(mac_address_wifi);
  Serial.print("device_id  :  "); Serial.println(device_id);

  BLEDevice::init(device_id.c_str());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());



  Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();


  // Start advertising
  pAdvertising->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {


  String light_sensor = String(random(1000, 8000));
  String ax = String(random(-100, 100));
  String ay = String(random(-100, 100));
  String az = String(random(-100, 100));
  String gx = String(random(-100, 100));
  String gy = String(random(-100, 100));


  String payload;
  payload.concat("W");
  payload.concat(",");
  payload.concat(mac_address_wifi);
  payload.concat(",");
  payload.concat(light_sensor);
  payload.concat(",");
  payload.concat(ax);
  payload.concat(",");
  payload.concat(ay);
  payload.concat(",");
  payload.concat(az);
  payload.concat(",");
  payload.concat(gx);
  payload.concat(",");
  payload.concat(gy);


  pCharacteristic->setValue(payload.c_str());
  pCharacteristic->notify();
  Serial.print("payload  :  "); Serial.println(payload);
  delay(10000);
}
