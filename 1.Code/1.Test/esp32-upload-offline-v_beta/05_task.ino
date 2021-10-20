void f_Task_Reconnect_wifi( void * pvParameters ) {
  Serial.print("Task_Reconnect_wifi running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {


    if (state_setup_wifi == false)
    {
      setup_wifi();
    }

    else {
#if mqtt
      if (!mqtt_wifi.connected()) {
        reconnect_wifi();
      }
      mqtt_wifi.loop();
#endif
    }
  }
}


void f_Task_dump( void * pvParameters ) {
  Serial.print("f_Task_dump running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
   // run_work();
  }
}

void f_Task_serial( void * pvParameters ) {
  Serial.print("f_Task_serial running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    cmd_serial();
  }
}
