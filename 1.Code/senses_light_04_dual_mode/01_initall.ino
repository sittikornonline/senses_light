void f_init_all()
{
  Serial.begin(115200);

  //------------- WiFI : MAC Address -------------//

  mac_address = WiFi.macAddress();
  mac_address.replace(":", "");
  last_3_mac = mac_address.substring(9, 12);
  gateway_id = "SEN_" + last_3_mac;


  //----------------- i2C -------------//
  Wire.begin (21, 22);
  Wire1.begin (26, 25);

  //----------------- Light : 6030 -------------//
  light_6030.begin();
  light_6030.setGain(gain);
  light_6030.setIntegTime(times);

  if (!RGBWSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
  }

  //----------------- Light : 6040 -------------//
  RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);


  //----------------- UV : 6075 -------------//
  uv.begin();


  //----------------- MPU : 6050 -------------//
  accelgyro.initialize();


  //----------------- LED RGB -------------------//
  ledcSetup(red_Channel, freq, resolution);
  ledcSetup(green_Channel, freq, resolution);
  ledcSetup(blue_Channel, freq, resolution);


  ledcAttachPin(red_pin,   red_Channel);
  ledcAttachPin(green_pin, green_Channel);
  ledcAttachPin(blue_pin,  blue_Channel);


  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  led_rgb(255, 255, 255);

  //----------------- V-Batt -------------------//

  analogReadResolution(10);
  pinMode(batt_pin, OUTPUT);


  //----------------- BLE -------------------//

  // Create the BLE Device
  BLEDevice::init(gateway_id.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_N12);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());


  //Create the BLE Service
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



  //----------------------- Task ---------------------------//

  xTaskCreatePinnedToCore(
    f_Task_ble_connect,   /* Task function. */
    "Task_ble_connect",     /* name of task. */
    5000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_ble_connect,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


  xTaskCreatePinnedToCore(
    f_Task_ble_non_connect,   /* Task function. */
    "Task_ble_non_connect",     /* name of task. */
    5000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task_ble_non_connect,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


}
