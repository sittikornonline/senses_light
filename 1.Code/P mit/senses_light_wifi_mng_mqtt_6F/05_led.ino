void led_rgb(int r, int g, int b)
{
  ledcWrite(red_Channel, r);
  ledcWrite(green_Channel, g);
  ledcWrite(blue_Channel, b);
}

void led_batt_full()
{
  Serial.println("led batt full");
  led_rgb(0, 255, 0);
}


void led_batt_low()
{
  Serial.println("led batt low");
  led_rgb(255, 0, 0);
}


void led_sync()
{
  Serial.println("led sync");
  led_rgb(0, 0, 255);
}
