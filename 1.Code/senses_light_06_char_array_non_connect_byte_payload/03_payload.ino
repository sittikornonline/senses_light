

void set_payload()
{

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

  }

  else
  {
    light_6030_raw = random(1, 800);
    light_6040_raw = random(1, 800);
    uv_a_raw = random(0, 255);
    uv_b_raw = random(0, 255);
    ax = random(-100, 100);
    ay = random(-100, 100);
    az = random(-100, 100);
    gx = random(-100, 100);
    gy = random(-100, 100);
    gz = random(-100, 100);

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

  if (cnt_advers == 1)
  {
    payload_beacon[0]  = (cnt_advers & 0xFF);

    payload_beacon[1]  = payload_mac[0];
    payload_beacon[2]  = payload_mac[1];
    payload_beacon[3]  = payload_mac[2];
    payload_beacon[4]  = payload_mac[3];
    payload_beacon[5]  = payload_mac[4];
    payload_beacon[6]  = payload_mac[5];

    payload_beacon[7]  = (light_6030_raw >> 8);
    payload_beacon[8]  = (light_6030_raw & 0xFF);

    payload_beacon[9]  = (light_6040_raw >> 8);
    payload_beacon[10] = (light_6040_raw & 0xFF);

    payload_beacon[11] = (uv_a_raw >> 8);
    payload_beacon[12] = (uv_a_raw & 0xFF);

    payload_beacon[13] = (uv_b_raw >> 8);
    payload_beacon[14] = (uv_b_raw & 0xFF);

    payload_beacon[15] = (v_batt >> 8);
    payload_beacon[16] = (v_batt & 0xFF);

    payload_beacon_length = 17;
  }

  else if (cnt_advers == 2)
  {
    payload_beacon[0]  = (cnt_advers & 0xFF);

    payload_beacon[1]  = payload_mac[0];
    payload_beacon[2]  = payload_mac[1];
    payload_beacon[3]  = payload_mac[2];
    payload_beacon[4]  = payload_mac[3];
    payload_beacon[5]  = payload_mac[4];
    payload_beacon[6]  = payload_mac[5];

    payload_beacon[7]  = (ax >> 8);
    payload_beacon[8]  = (ax & 0xFF);

    payload_beacon[9]  = (ay >> 8);
    payload_beacon[10] = (ay & 0xFF);

    payload_beacon[11] = (az >> 8);
    payload_beacon[12] = (az & 0xFF);

    payload_beacon[13] = (batt_full & 0xFF);

    payload_beacon_length = 14;
  }

  Serial.print("cnt_advers  : "); Serial.println(cnt_advers);
  Serial.print("mac_address : "); Serial.println(mac_address);
  Serial.print("gateway_id  : "); Serial.println(gateway_id);
  Serial.print("6030 AL     : "); Serial.print(light_6030_raw); Serial.println(" lux");
  Serial.print("6040 AL     : "); Serial.print(light_6040_raw); Serial.println(" lux");
  Serial.print("6075 UVA    : "); Serial.println(uv_a_raw);
  Serial.print("6075 UVB    : "); Serial.println(uv_b_raw);
  Serial.print("v_batt      : "); Serial.println(v_batt);
  Serial.print("batt_full   : "); Serial.println(batt_full);

  Serial.print("6050 MPU    : ");
  Serial.print(ax); Serial.print(",");  Serial.print(ay); Serial.print(","); Serial.println(az);

  Serial.print("payload_beacon : ");
  for (int i = 0; i < payload_beacon_length; i++)
  {
    Serial.print(payload_beacon[i], HEX); Serial.print("-");
  }

  Serial.println("\n---------------------------------------------");
  cnt_advers ++;

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
  unsigned int mac_1_int = mac_1_str.toInt();

  String mac_2_str;
  mac_2_str =  x2i(str2charArray(mac_2));
  unsigned int mac_2_int = mac_2_str.toInt();


  payload_mac[6];
  payload_mac[0] = (mac_1_int >> 16);
  payload_mac[1] = (mac_1_int >> 8);
  payload_mac[2] = (mac_1_int & 0xFF);
  payload_mac[3] = (mac_2_int >> 16);
  payload_mac[4] = (mac_2_int >> 8);
  payload_mac[5] = (mac_2_int & 0xFF);


  Serial.print("mac_full       : "); Serial.println(mac_1 + mac_2);
  Serial.print("payload_mac[0] : "); Serial.println(payload_mac[0], HEX);
  Serial.print("payload_mac[1] : "); Serial.println(payload_mac[1], HEX);
  Serial.print("payload_mac[2] : "); Serial.println(payload_mac[2], HEX);
  Serial.print("payload_mac[3] : "); Serial.println(payload_mac[3], HEX);
  Serial.print("payload_mac[4] : "); Serial.println(payload_mac[4], HEX);
  Serial.print("payload_mac[5] : "); Serial.println(payload_mac[5], HEX);
  Serial.println("---------------------------------------------");
}
