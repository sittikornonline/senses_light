#define red_pin 2
#define green_pin 15
#define blue_pin 4

const int freq = 5000;

const int red_Channel = 0;
const int green_Channel = 1;
const int blue_Channel = 2;
const int resolution = 8;

void setup() {
  // put your setup code here, to run once:

  ledcSetup(red_Channel, freq, resolution);
  ledcSetup(green_Channel, freq, resolution);
  ledcSetup(blue_Channel, freq, resolution);

  
  ledcAttachPin(red_pin,   red_Channel);
  ledcAttachPin(green_pin, green_Channel);
  ledcAttachPin(blue_pin,  blue_Channel);

  Serial.begin(115200);
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

}

void loop() {

  led_rgb(255, 0, 0);  delay(1000);
  led_rgb(0, 255, 0);  delay(1000);
  led_rgb(0, 0, 255);  delay(1000);
  led_rgb(255, 255, 255);  delay(1000);
  led_rgb(0, 0, 0);  delay(1000);

}


void led_rgb(int r, int g, int b)
{
  ledcWrite(red_Channel, r);
  ledcWrite(green_Channel, g);
  ledcWrite(blue_Channel, b);
}
