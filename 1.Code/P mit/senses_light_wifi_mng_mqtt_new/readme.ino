/*
   03-12-2021 : Change battery full charge state indicator -> Green LED allway ON.
*/

/* start 21.10 - 21-11-2021
  -------------------------  WAKEUP TIME  ---------------------------
  WIFI_ADDRESS : 308398D9FD54
  GATEWAY_ID   : SEN_D54
  6030 AL      : 617
  6040 R       : 11213
  6040 G       : 11967
  6040 B       : 5966
  6040 W       : 35631
  6075 UVA     : 47.28
  6075 UVB     : 86.10
  TEMPERATURE  : 40.46
  BATT V       : 4.20 V
  ERROR CODE   : 0x00
  BLE PAYLOAD  : 02:69:2B:CD:2E:BF:17:4E:8B:2F:12:78:21:A2:0F:CD:00
  WAKEUP TIME  : 7281
  SLEEP TIME   : 172719
  -----------------------------  END  -------------------------------
*/




/*
   RGB LED Status
   if VBATT > 3.60V & Battery charge Full - All way On Green LED.
   if VBATT > 3.60V - Blink Blue LED 2 times.
   if VBATT > 3.50V - Blink Red LED 2 times.
   if VBATT > 3.45V - Blink Red LED 1 times.
   if VBATT < 3.45V - Blink Red LED 1 times and set LOWBATT in Error code.

   VDD supply voltage range for sensers.
   VEML6030 : Operation voltage: 2.5 V to 3.6 V
   VEML6040 : Operation voltage: 2.5 V to 3.6 V
   VEML6075 : Operation voltage: 1.7 V to 3.6 V
   MPU6050  : Operation voltage: 2.37V to 3.46V
   ESP32-PICO-D4  : Operation voltage: 3.0 V to 3.6 V

*/



/* ESP32 BOOT MESSAGE
  ets Jun  8 2016 00:22:57 = 26 byte

  rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT) = 58 BYTE = 68
  configsip: 188777542, SPIWP:0xee = 33 BYTE = 44
  clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00 = 73
  mode:DIO, clock div:1 = 23
  load:0x3fff0018,len:4 = 23
  load:0x3fff001c,len:1216 = 26
  ho 0 tail 12 room 4 = 21
  load:0x40078000,len:10944 = 27
  load:0x40080400,len:6388 = 26
  entry 0x400806b4 = 18
  TOTAL = 375 BYTE = +/- 26 MSEC
  = 1/115200 = 8.680555555555556e-6 * 8 = 6.944444444444444e-5 / BYTE
  = 6.944444444444444e-5 * 375 = 0.0260416666666667 = 26.04 MSEC

*/



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


*/
