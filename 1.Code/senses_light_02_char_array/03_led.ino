void led_rgb(int r, int g, int b)
{
  ledcWrite(red_Channel, r);
  ledcWrite(green_Channel, g);
  ledcWrite(blue_Channel, b);
}

void led_batt_full()
{
  led_rgb(255, 100, 255);
}


void led_batt_low()
{
  led_rgb(100, 100, 255);
}
