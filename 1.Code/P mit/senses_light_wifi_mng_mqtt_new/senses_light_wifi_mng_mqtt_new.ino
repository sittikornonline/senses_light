#include <stdio.h>
#include "header.h"
#include "Arduino.h"

// Util Functions
void blink(uint8_t count, uint8_t pin) {
  uint8_t state = HIGH;

  for (int x = 0; x < (count << 1); ++x) {
    digitalWrite(pin, state ^= HIGH);
    delay(75);
  }
  digitalWrite(pin, HIGH);
}

void setup() {


  initall();
  startMillis = millis();

#if 1
  esp_reset_reason_t reason = esp_reset_reason();

  /* Reset when the vdd voltage is not stable */
  if (reason == ESP_RST_BROWNOUT) {
    esp_deep_sleep(1000000LL * 3600000);    // long time sleep 1 hour.

  } else if (reason == ESP_RST_POWERON) {
    gyroOffSetFlag = true;
  }

#endif


  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));


  //Set timer to 5 seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");





}
void loop() {

  if (test_max_log == false)
  {

    if (flag_upload_when_night == true && c_setup_wifi == true)
    {
      if (status_mqtt == true && cnt_payload > 0)
      {
        code_cmd_spiff = 3; // shift data
        flag_spiff = true;
      }

      else if (status_mqtt == true && emptyFile == true && emptyFile_2 == true && emptyFile_3 == true)
      {
        ESP.restart();
      }
    }


    else if (flag_upload_when_night == false)
    {
      code_cmd_spiff = 1; // put data
      flag_spiff = true;
      flag_putdata = true;

    }
  }

}
