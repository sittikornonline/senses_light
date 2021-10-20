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

  oScanResponseData.setServiceData(BLEUUID(beconUUID), std::string(payload_beacon, payload_length)); 
  oAdvertisementData.setName(gateway_id.c_str());
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}
