void setBeacon()
{

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  oScanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  oScanResponseData.setCompleteServices(BLEUUID(beconUUID));

  String payload  = set_payload();
  int payload_length = payload.length();


  payload_beacon[sizeof(payload)];
  payload.toCharArray(payload_beacon, sizeof(payload_beacon));

  //  Serial.print("--> payload         : "); Serial.println(payload);
  //  Serial.print("--> payload_beacon  : "); Serial.println(payload_beacon);

  //oScanResponseData.setServiceData(BLEUUID(beconUUID), std::string(payload_beacon, payload_length));
   oScanResponseData.setServiceData(BLEUUID(beconUUID), std::string("123456789012345678901"));
  oAdvertisementData.setName(gateway_id.c_str());
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}
