void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  debugln("");
  debug("Connecting to ");
  debugln(ssid);

  WiFi.begin(ssid, passw);
  // WiFi.begin(ssid, passw);

  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    debug("connecting ");   debug(ssid); debug(",");     debug(passw); debugln(",");
  }

  debugln("");
  debugln("WiFi connected");
  debugln("IP address: "); debugln(String(WiFi.localIP()));

  if (String(WiFi.localIP()) != "0.0.0.0")
  {
    state_setup_wifi = true;
    status_conneect_wifi = true;

    ntpClient.begin();
    configTime(0, 0, NTP_SERVER);
  }

}



void reconnect_wifi() {

  while (!mqtt_wifi.connected()) {
    //debug("Attempting MQTT connection...");
    // Attempt to connect

    if (mqtt_wifi.connect(mac_address.c_str())) {
      debugln("connected");
      // Subscribe
      mqtt_wifi.subscribe(mqtt_sub.c_str());
      status_mqtt = true;
      debug("1 status_mqtt = ");  debugln(String(status_mqtt));
    }
    else {
      status_mqtt = false;
      debug("2 status_mqtt = ");  debugln(String(status_mqtt));
      debug("failed, rc=");
      debug(String(mqtt_wifi.state()));
      debugln(" try again in 5 seconds");
      // Wait 5 seconds before retrying

      delay(5000);
      cnt_wifi_disconect ++;
      if (cnt_wifi_disconect >= 5) {
        setup_wifi();
        cnt_wifi_disconect = 0;
      }
    }
  }
}



void mqttCallback(char* topic, byte* message, unsigned int length) {
  debug("Message arrived on topic: ");
  debug(String(topic));
  debug(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    debug(String((char)message[i]));
    messageTemp += (char)message[i];
  }
  debugln("");

  if (String(topic) == mqtt_sub) {
    debug("cmd :  ");
    if (messageTemp == "on") {
      debugln("on"); ;
    }
    else if (messageTemp == "off") {
      debugln("off");
    }

    else if (messageTemp == "hb" ) {
      debugln(": hb");
      mqtt_wifi.publish(mqtt_pub.c_str(), setPayload_hb().c_str());
      debug("Mode HB - payload : "); debugln(String(setPayload_hb()));
    }



  }
}

boolean mqttConnect() {
  debug("Connecting to ");
  debug(mqtt_broker);


  // Connect to MQTT Broker
  boolean status = mqtt_wifi.connect(mqtt_broker);
  status_network = status;

  // Or, if you want to authenticate MQTT:
  //boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

  if (status == false) {
    debugln(" fail");
    status_network = status;
    return false;
  }
  debugln(" success");
  String online = mac_address + ":online";

  mqtt_wifi.publish(mqtt_pub.c_str(), online.c_str());
  mqtt_wifi.subscribe(mqtt_sub.c_str());
  return mqtt_wifi.connected();



}



String setPayload_w() {

  String mode = "W";
  String payload;
  payload.reserve(500);

  payload.concat(mode);
  payload.concat(":");
  payload.concat(mac_address);
  payload.concat(":");  
  payload.concat(payload_mqtt);
  return payload;
}





String setPayload_hb() {

  String mode = "HB";
  String payload = "";
  payload.concat(mode);
  payload.concat(":");
  payload.concat(mac_address);
  return payload;
}



void sendPayload() {


  //   Mode W : Sent data on canbus to server
  //  unsigned int now_mqtt_w = millis();
  //  if (now_mqtt_w - lastMsg_mqtt_w > interval_mqtt_w) {
  //    lastMsg_mqtt_w = now_mqtt_w;
  //    mqtt_wifi.publish(mqtt_pub.c_str(), setPayload_w().c_str());
  //    debug("Mode W - payload : "); debugln(setPayload_w());
  //  }

  //  if (status_send_w == true) // force send
  //  {
  //    bool status_send = mqtt_wifi.publish(mqtt_pub.c_str(), setPayload_w().c_str());
  //    debug("Mode W - payload : "); debugln(setPayload_w());
  //    debug("Mode W - status  : "); debugln(String(status_send));
  //    delay(1000);
  //    status_send_w = false;
  //  }





  //Mode HB : Sent data on canbus other to server
  //  unsigned int now_mqtt_hb = millis();
  //  if (now_mqtt_hb - lastMsg_mqtt_hb > interval_mqtt_hb) {
  //    lastMsg_mqtt_hb = now_mqtt_hb;
  //    mqtt_wifi.publish(mqtt_pub.c_str(), setPayload_hb().c_str());
  //    debug("Mode HB - payload : "); debugln(setPayload_hb());
  //  }

}
