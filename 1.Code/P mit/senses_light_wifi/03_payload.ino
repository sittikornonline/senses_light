/* Macros to access bytes within words and words within longs */
#define LOW_BYTE(x)     ((uint8_t)((x)&0xFF))
#define HIGH_BYTE(x)    ((uint8_t)(((x)>>8)&0xFF))
#define LOW_WORD(x)     ((uint16_t)((x)&0xFFFF))
#define HIGH_WORD(x)    ((uint16_t)(((x)>>16)&0xFFFF))

#if 0
void ble_mac2String(){
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
#ifdef FIRMWARE_X
  int16_t temp_16t;
  if (mc_mode == 2){
    rendom();
  } else {
    temp_16t = LOW_WORD(light_6030_raw);       // need only low word of light_6030_raw. convert long to int16_t.    
  }
  payload_beacon[0] = HIGH_BYTE(temp_16t);
  payload_beacon[1] = LOW_BYTE(temp_16t);

  payload_beacon[2] = HIGH_BYTE(VEML6040_red);
  payload_beacon[3] = LOW_BYTE(VEML6040_red);

  payload_beacon[4] = HIGH_BYTE(VEML6040_green);
  payload_beacon[5] = LOW_BYTE(VEML6040_green);

  payload_beacon[6] = HIGH_BYTE(VEML6040_blue);
  payload_beacon[7] = LOW_BYTE(VEML6040_blue);

  payload_beacon[8] = HIGH_BYTE(VEML6040_white);
  payload_beacon[9] = LOW_BYTE(VEML6040_white);

  temp_16t = (int16_t)uv_a_raw;
  payload_beacon[10] = HIGH_BYTE(temp_16t);
  payload_beacon[11] = LOW_BYTE(temp_16t);

  temp_16t = (int16_t)uv_b_raw;
  payload_beacon[12] = HIGH_BYTE(temp_16t);
  payload_beacon[13] = LOW_BYTE(temp_16t );

  temp_16t = (int16_t)(mpu6050Data*100);                   // Make 12.34 to 1234 for int 16 bit format.  
  payload_beacon[14] = HIGH_BYTE(temp_16t);
  payload_beacon[15] = LOW_BYTE(temp_16t);

  payload_beacon[16] = Error_code;        // Send ERROR code

  payload_beacon_length = 17;

  if(bleAdverFlag) {  
    Serial.print("BLE_ADDRESS  : "); Serial.println(ble_address);
    Serial.print("DEVICE NAME  : "); Serial.println(gateway_id);
    Serial.print("PERIOD TIME  : "); Serial.println(GPIO_DEEP_SLEEP_DURATION/1000);
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
  }
/*  
-------------------------  WAKEUP TIME  --------------------------- = 69
WIFI_ADDRESS : 308398D9FD54 = 29
GATEWAY_ID   : SEN_D54 = 24
6030 AL      : 583 = 20
6040 R       : 12023 = 22
6040 G       : 12449 = 22
6040 B       : 6343  = 22
6040 W       : 37906 = 22
6075 UVA     : 42.51 = 22
6075 UVB     : 79.05 = 22
TEMPERATURE  : 35.66 = 22
BATT V       : 3.90  = 21
ERROR CODE   : 0x00  = 21
BLE PAYLOAD  : 02:47:2E:F7:30:A1:18:C7:94:12:10:9B:1E:E1:0D:ED:00 = 67
WAKEUP TIME  : 7132 = 21
SLEEP TIME   : 172868 = 23
-----------------------------  END  ------------------------------- = 69
TOTAL = 518 BYTE = +/- 40 MSEC
  = 1/115200 = 8.680555555555556e-6 * 8 = 6.944444444444444e-5 / BYTE
  = 6.944444444444444e-5 * 518 = 0.0359722222222222 = 35.97 MSEC

*   
*/

  Serial.print("BLE PAYLOAD  : ");
  for (int i = 0; i < payload_beacon_length; i++)
  {
    Serial.print(payload_beacon[i] < 0x10 ? "0" : "");
    Serial.print(payload_beacon[i], HEX); Serial.print(i < payload_beacon_length-1 ? ":" : "\n");
  }

//  Serial.println("-------------------------------------------------------------------");

/*********************************************************************************** ORG FIRMWARE ****************************************************************************************************/
#else

  if (mc_mode == 1)
  {
    light_6030_raw = light_6030.readLight();
    light_6040_raw = RGBWSensor.getAmbientLight();
    uv_a_raw = uv.uva();
    uv_b_raw = uv.uvb();
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    v_batt = analogRead(v_batt_pin);
    v_batt = map(v_batt, 0, 6.6, 0, 100);
    batt_full = digitalRead(batt_pin);

    delay(800);
    digitalWrite(power_sensor_pin, LOW);

  }

  else
  {
    light_6030_raw = random(1, 800);
    light_6040_raw = random(1, 800);
    uv_a_raw = random(0, 255);
    uv_b_raw = random(0, 255);
    ax = random(-10, 100);
    ay = random(-10, 100);
    az = random(-10, 100);
    gx = random(-10, 100);
    gy = random(-10, 100);
    gz = random(-10, 100);

    v_batt = analogRead(v_batt_pin);
    v_batt = map(v_batt, 0, 6.6, 0, 100);
    batt_full = digitalRead(batt_pin);
  }

  if (batt_full == 0)
  {
    led_batt_full();
    batt_full = 1;
  }
  else if (batt_full == 1)
  {
    led_batt_low();
    batt_full = 0;
  }

  payload_beacon[0]  = (light_6030_raw >> 8);
  payload_beacon[1]  = (light_6030_raw & 0xFF);

  payload_beacon[2]  = (light_6040_raw >> 8);
  payload_beacon[3] = (light_6040_raw & 0xFF);

  payload_beacon[4] = (uv_a_raw >> 8);
  payload_beacon[5] = (uv_a_raw & 0xFF);

  payload_beacon[6] = (uv_b_raw >> 8);
  payload_beacon[7] = (uv_b_raw & 0xFF);

  payload_beacon[8] = (v_batt >> 8);
  payload_beacon[9] = (v_batt & 0xFF);

  payload_beacon[10]  = (ax >> 8);
  payload_beacon[11]  = (ax & 0xFF);

  payload_beacon[12]  = (ay >> 8);
  payload_beacon[13]  = (ay & 0xFF);

  payload_beacon[14]  = (az >> 8);
  payload_beacon[15]  = (az & 0xFF);

  payload_beacon[16]  = (batt_full & 0xFF);

  payload_beacon_length = 17;


   
  Serial.print("wifi_address : "); Serial.println(mac_address);
  Serial.print("gateway_id   : "); Serial.println(gateway_id);
  Serial.print("6030 AL      : "); Serial.print(light_6030_raw); Serial.println(" lux");
  Serial.print("6040 AL      : "); Serial.print(light_6040_raw); Serial.println(" lux");
  Serial.print("6075 UVA     : "); Serial.println(uv_a_raw);
  Serial.print("6075 UVB     : "); Serial.println(uv_b_raw);
  Serial.print("v_batt       : "); Serial.println(v_batt);
  Serial.print("batt_full    : "); Serial.println(batt_full);

  Serial.print("6050 MPU     : ");
  Serial.print(ax); Serial.print(",");  Serial.print(ay); Serial.print(","); Serial.println(az);

  Serial.print("BLE PAYLOAD  : ");
  for (int i = 0; i < payload_beacon_length; i++)
  {
    Serial.print(payload_beacon[i], HEX); Serial.print(":");
  }

  Serial.println("\n-------------------------------------------------------------------");

#endif

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
