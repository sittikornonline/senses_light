String set_payload()
{

  if (mc_mode == 1)
  {
    light_6030_raw = light_6030.readLight();
    light_6040_raw = RGBWSensor.getAmbientLight();
    uv_a_raw = uv.uva();
    uv_b_raw = uv.uvb();
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
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
  }


  String payload;
  payload.concat("W");
  payload.concat(",");
  payload.concat(mac_address);
  payload.concat(",");
  payload.concat(light_6030_raw);
  payload.concat(",");
  payload.concat(light_6040_raw);
  payload.concat(",");
  payload.concat(uv_a_raw);
  payload.concat(",");
  payload.concat(uv_b_raw);
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
  payload.concat(",");
  payload.concat(gz);


  Serial.print("mac_address : "); Serial.println(mac_address);
  Serial.print("gateway_id  : "); Serial.println(gateway_id);
  Serial.print("6030 AL     : "); Serial.print(light_6030_raw); Serial.println(" lux");
  Serial.print("6040 AL     : "); Serial.print(light_6040_raw); Serial.println(" lux");
  Serial.print("6075 UVA    : "); Serial.println(uv_a_raw);
  Serial.print("6075 UVB    : "); Serial.println(uv_b_raw);

  Serial.print("6050 MPU    : ");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  Serial.print("payload     : "); Serial.println(payload);
  //Serial.println("--------------------------------------------------");

  return payload;
}
