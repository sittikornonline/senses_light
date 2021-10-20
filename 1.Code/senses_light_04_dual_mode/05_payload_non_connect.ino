String set_payload_non_connect()
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
    light_6030_raw = random(1, 800);
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

  String payload;

  if (cnt_advers == 1)
  {
    payload = "";
    payload.concat(last_3_mac);
    payload.concat(",");
    payload.concat(cnt_advers);
    payload.concat(",");
    payload.concat(light_6030_raw);
    payload.concat(",");
    payload.concat(light_6040_raw);

  }

  else if (cnt_advers == 2)
  {
    payload = "";
    payload.concat(last_3_mac);
    payload.concat(",");
    payload.concat(cnt_advers);
    payload.concat(",");
    payload.concat(uv_a_raw);
    payload.concat(",");
    payload.concat(uv_b_raw);
    payload.concat(",");
    payload.concat(v_batt);
  }

  else if (cnt_advers == 3)
  {
    payload = "";
    payload.concat(last_3_mac);
    payload.concat(",");
    payload.concat(cnt_advers);
    payload.concat(",");
    payload.concat(ax);
    payload.concat(",");
    payload.concat(ay);
    payload.concat(",");
    payload.concat(batt_full);
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
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.println(az); 
 

  Serial.print("payload     : "); Serial.println(payload); 
  Serial.print("length      : "); Serial.println(payload.length());
  Serial.println("---------------------------------------------");
  cnt_advers ++;
  return payload;
}

 
