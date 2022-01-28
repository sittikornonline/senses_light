/* Macros to access bytes within words and words within longs */
#define LOW_BYTE(x)     ((uint8_t)((x)&0xFF))
#define HIGH_BYTE(x)    ((uint8_t)(((x)>>8)&0xFF))
#define LOW_WORD(x)     ((uint16_t)((x)&0xFFFF))
#define HIGH_WORD(x)    ((uint16_t)(((x)>>16)&0xFFFF))

#if 0
void ble_mac2String() {
  const uint8_t* point = esp_bt_dev_get_address();
  //  String s;
  for (byte i = 0; i < 6; ++i)
  {
    char str[3];
    sprintf(str, "%2X", (int)point[i]);
    mac_address += str;
    if (i < 5) mac_address += ':';
  }
  //  return s;
}


void printDeviceAddress() {

  const uint8_t* point = esp_bt_dev_get_address();

  for (int i = 0; i < 6; i++) {

    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    ble_address += str;
    //    Serial.print(str);

    //    if (i < 5){
    //      Serial.print(":");
    //    }

  }
  //  Serial.print("\n");
  Serial.println(ble_address);
}
#endif
void rendom()
{
  light_6030_raw = random(1, 5000);
  VEML6040_red = random(1, 65535);
  VEML6040_green = random(1, 65535);
  VEML6040_blue = random(1, 65535);
  VEML6040_white = random(1, 65535);
  uv_a_raw = (float)random(0, 300.00);
  uv_b_raw = (float)random(0, 300.00);
  mpu6050Data = (float)random(0, 200.05);
  Error_code = 0x00;
}


void set_payload()
{
  /*********************************************************************************** FIRMWARE_X ****************************************************************************************************/

  int16_t temp_16t;
  if (mc_mode == 2) {
    rendom();
  } else {
    temp_16t = LOW_WORD(light_6030_raw);       // need only low word of light_6030_raw. convert long to int16_t.
  }


  String _cnt_payload = EEPROM.readString(add_cnt_payload);
  int _cnt_payload_int = _cnt_payload.toInt();
  if (_cnt_payload_int <= 0 || _cnt_payload == "")
  {
    cnt_payload = 0;
  }
  cnt_payload = _cnt_payload_int;
  cnt_payload++;
  EEPROM.writeString(add_cnt_payload, String(cnt_payload));
  EEPROM.commit();

  if (debug_sensor == true) {
    Serial.print("DEVICE NAME  : "); Serial.println(gateway_id);
    Serial.print("PERIOD TIME  : "); Serial.println(GPIO_DEEP_SLEEP_DURATION / 1000);
    Serial.println("6030 AL      : " + String(light_6030_raw));
    Serial.println("6040 R       : " + String(VEML6040_red));
    Serial.println("6040 G       : " + String(VEML6040_green));
    Serial.println("6040 B       : " + String(VEML6040_blue));
    Serial.println("6040 W       : " + String(VEML6040_white));
    Serial.println("6075 UVA     : " + String((float)uv_a_raw / 100));
    Serial.println("6075 UVB     : " + String((float)uv_b_raw / 100));
    Serial.println("ACC ANGLE X  : " + String(mpu6050Data));
    Serial.println("BATTERY V    : " + String(batt_voltage, 3));
    Serial.print("CHARGE STA   : "); Serial.println(digitalRead(CHG_PIN) ? "CHRG" : "FULL");
    Serial.print("ERROR CODE   : 0x"); Serial.print(Error_code < 0x10 ? "0" : ""); Serial.println(Error_code, HEX);
    Serial.println("CNT PAY    : " + String(cnt_payload));
    Serial.println("----------------------------------------");



    payload_str.concat(String(getTime_str()));
    payload_str.concat(":");
    payload_str.concat(String(light_6030_raw));
    payload_str.concat(":");
    payload_str.concat(String(VEML6040_red));
    payload_str.concat(":");
    payload_str.concat(String(VEML6040_green));
    payload_str.concat(":");
    payload_str.concat(String(VEML6040_blue));
    payload_str.concat(":");
    payload_str.concat(String(VEML6040_white));
    payload_str.concat(":");
    payload_str.concat(String((float)uv_a_raw / 100));
    payload_str.concat(":");
    payload_str.concat(String((float)uv_b_raw / 100));
    payload_str.concat(":");
    payload_str.concat(String(mpu6050Data));
    payload_str.concat(":");
    payload_str.concat(String(batt_voltage, 2));
    payload_str.concat(":");
    payload_str.concat(String(cnt_payload));
    payload_str.concat("#");



    String _cnt_group_payload = EEPROM.readString(add_cnt_group_payload);
    int _cnt_group_payload_int = _cnt_group_payload.toInt();
    if (_cnt_group_payload_int <= 0 || _cnt_group_payload == "")
    {
      cnt_group_payload = 0;
    }

    cnt_group_payload = _cnt_group_payload_int;
    cnt_group_payload++;
    EEPROM.writeString(add_cnt_group_payload, String(cnt_group_payload));
    EEPROM.commit();


    if (cnt_group_payload >= max_group_payload)
    {
      payload_str.concat("@");
      cnt_group_payload = 1;
      EEPROM.writeString(add_cnt_group_payload, String(cnt_group_payload));
      EEPROM.commit();
    }
  }

}




char* str2charArray(String str) {

  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);


}

long x2i(char *s)
{
  long x = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
    }
    else break;
    s++;
  }

  return x;
}


void mac_to_byte_array()
{
  String mac_1 = mac_address.substring(0, 6);
  String mac_2 = mac_address.substring(6, 12);

  String mac_1_str;
  mac_1_str =  x2i(str2charArray(mac_1));
  int mac_1_int = mac_1_str.toInt();

  String mac_2_str;
  mac_2_str =  x2i(str2charArray(mac_2));
  int mac_2_int = mac_2_str.toInt();


  payload_mac[6];
  payload_mac[0] = (mac_1_int >> 16);
  payload_mac[1] = (mac_1_int >> 8);
  payload_mac[2] = (mac_1_int & 0xFF);
  payload_mac[3] = (mac_2_int >> 16);
  payload_mac[4] = (mac_2_int >> 8);
  payload_mac[5] = (mac_2_int & 0xFF);

  /*
    Serial.print("mac_full       : "); Serial.println(mac_1 + mac_2);
    Serial.print("payload_mac[0] : "); Serial.println(payload_mac[0], HEX);
    Serial.print("payload_mac[1] : "); Serial.println(payload_mac[1], HEX);
    Serial.print("payload_mac[2] : "); Serial.println(payload_mac[2], HEX);
    Serial.print("payload_mac[3] : "); Serial.println(payload_mac[3], HEX);
    Serial.print("payload_mac[4] : "); Serial.println(payload_mac[4], HEX);
    Serial.print("payload_mac[5] : "); Serial.println(payload_mac[5], HEX);
    Serial.println("---------------------------------------------");
  */
}
