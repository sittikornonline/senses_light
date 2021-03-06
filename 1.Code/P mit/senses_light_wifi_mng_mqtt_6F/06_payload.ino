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
  mpu6050Data_x = (float)random(0, 200.05);
  mpu6050Data_y = (float)random(0, 200.05);
  mpu6050Data_z = (float)random(0, 200.05);
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


  if (test_max_log == false)
  {
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

    _cnt_payload = EEPROM.readString(add_cnt_payload);
  }

  if (debug_sensor == true) {
    Serial.print("DEVICE NAME     : "); Serial.println(gateway_id);
    Serial.print("PERIOD TIME     : "); Serial.println(GPIO_DEEP_SLEEP_DURATION / 1000);
    Serial.println("6030 AL       : " + String(light_6030_raw));
    Serial.println("6040 R        : " + String(VEML6040_red));
    Serial.println("6040 G        : " + String(VEML6040_green));
    Serial.println("6040 B        : " + String(VEML6040_blue));
    Serial.println("6040 W        : " + String(VEML6040_white));
    Serial.println("6075 UVA      : " + String((float)uv_a_raw / 100));
    Serial.println("6075 UVB      : " + String((float)uv_b_raw / 100));
    Serial.println("ACC ANGLE X   : " + String(mpu6050Data_x));
    Serial.println("ACC ANGLE Y   : " + String(mpu6050Data_y));
    Serial.println("ACC ANGLE Z   : " + String(mpu6050Data_z));
    Serial.println("BATTERY V     : " + String(batt_voltage, 3));
    Serial.print("CHARGE STA      : "); Serial.println(digitalRead(CHG_PIN) ? "CHRG" : "FULL");
    Serial.print("ERROR CODE      : 0x"); Serial.print(Error_code < 0x10 ? "0" : ""); Serial.println(Error_code, HEX);
    Serial.println("CNT PAY       : " + String(cnt_payload));
    Serial.println("----------------------------------------");
  }


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
  payload_str.concat(String(mpu6050Data_x));
  payload_str.concat(":");
  payload_str.concat(String(mpu6050Data_y));
  payload_str.concat(":");
  payload_str.concat(String(mpu6050Data_z));
  payload_str.concat(":");
  payload_str.concat(String(batt_voltage, 2));
  payload_str.concat(":");
  payload_str.concat(String(cnt_payload));
  payload_str.concat("@");

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

String test_larg_payload()
{
  String payload = "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.22:1@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.23:2@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.24:3@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.25:4@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.26:5@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.27:6@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.28:7@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.29:8@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.30:9@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.31:10@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.32:11@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.33:12@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.34:13@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.35:14@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.36:15@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.37:16@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.38:17@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.39:18@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.40:19@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.41:20@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.42:21@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.43:22@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.44:23@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.45:24@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.46:25@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.47:26@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.48:27@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.49:28@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.50:29@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.51:30@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.52:31@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.53:32@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.54:33@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.55:34@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.56:35@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.57:36@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.58:37@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.59:38@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.60:39@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.61:40@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.62:41@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.63:42@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.64:43@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.65:44@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.66:45@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.67:46@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.68:47@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.69:48@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.70:49@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.71:50@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.72:51@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.73:52@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.74:53@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.75:54@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.76:55@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.77:56@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.78:57@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.79:58@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.80:59@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.81:60@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.82:61@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.83:62@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.84:63@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.85:64@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.86:65@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.87:66@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.88:67@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.89:68@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.90:69@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.91:70@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.92:71@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.93:72@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.94:73@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.95:74@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.96:75@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.97:76@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.98:77@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.99:78@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.100:79@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.101:80@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.102:81@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.103:82@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.104:83@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.105:84@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.106:85@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.107:86@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.108:87@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.109:88@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.110:89@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.111:90@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.112:91@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.113:92@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.114:93@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.115:94@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.116:95@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.117:96@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.118:97@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.119:98@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.120:99@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.121:100@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.122:101@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.123:102@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.124:103@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.125:104@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.126:105@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.127:106@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.128:107@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.129:108@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.130:109@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.131:110@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.132:111@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.133:112@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.134:113@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.135:114@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.136:115@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.137:116@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.138:117@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.139:118@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.140:119@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.141:120@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.142:121@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.143:122@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.144:123@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.145:124@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.146:125@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.147:126@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.148:127@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.149:128@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.150:129@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.151:130@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.152:131@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.153:132@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.154:133@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.155:134@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.156:135@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.157:136@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.158:137@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.159:138@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.160:139@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.161:140@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.162:141@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.163:142@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.164:143@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.165:144@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.166:145@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.167:146@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.168:147@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.169:148@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.170:149@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.171:150@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.172:151@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.173:152@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.174:153@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.175:154@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.176:155@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.177:156@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.178:157@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.179:158@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.180:159@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.181:160@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.182:161@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.183:162@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.184:163@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.185:164@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.186:165@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.187:166@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.188:167@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.189:168@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.190:169@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.191:170@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.192:171@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.193:172@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.194:173@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.195:174@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.196:175@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.197:176@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.198:177@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.199:178@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.200:179@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.201:180@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.202:181@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.203:182@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.204:183@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.205:184@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.206:185@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.207:186@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.208:187@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.209:188@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.210:189@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.211:190@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.212:191@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.213:192@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.214:193@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.215:194@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.216:195@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.217:196@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.218:197@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.219:198@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.220:199@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.221:200@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.222:201@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.223:202@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.224:203@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.225:204@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.226:205@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.227:206@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.228:207@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.229:208@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.230:209@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.231:210@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.232:211@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.233:212@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.234:213@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.235:214@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.236:215@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.237:216@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.238:217@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.239:218@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.240:219@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.241:220@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.242:221@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.243:222@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.244:223@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.245:224@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.246:225@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.247:226@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.248:227@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.249:228@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.250:229@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.251:230@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.252:231@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.253:232@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.254:233@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.255:234@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.256:235@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.257:236@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.258:237@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.259:238@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.260:239@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.261:240@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.262:241@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.263:242@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.264:243@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.265:244@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.266:245@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.267:246@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.268:247@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.269:248@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.270:249@\n"
                   "1643539538:85:1777:2071:893:3585:0.00:2.00:-0.00:4.271:250@\n";



  return payload;

  //  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
  //    Serial.println("SPIFFS Mount Failed");
  //    return;
  //  }
  //
  //  String dataFileName = payload;
  //
  //  int lengths = dataFileName.length() + 1;
  //  char dataFileName1[lengths];
  //
  //  dataFileName.toCharArray(dataFileName1, lengths);
  //  spiff.appendFile(SPIFFS, "/buffer.txt", dataFileName1);
  //  Serial.print("appendFile => ");   Serial.println(dataFileName1);
}
