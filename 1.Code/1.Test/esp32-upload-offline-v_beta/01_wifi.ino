#if wifi
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, passw);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("connecting ");   Serial.print(ssid); Serial.print(",");     Serial.print(passw); Serial.println(",");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");  Serial.println(WiFi.localIP());
  Serial.print("MacAddress: ");  Serial.println(WiFi.macAddress());
  state_setup_wifi = true;
}
#endif

#if mqtt
void reconnect_wifi() {

  while (!mqtt_wifi.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_wifi.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      mqtt_wifi.subscribe(mqtt_sub.c_str());
      status_mqtt = true;
      Serial.print("1 status_mqtt = ");  Serial.println(status_mqtt);
    } else {
      status_mqtt = false;
      Serial.print("2 status_mqtt = ");  Serial.println(status_mqtt);
      Serial.print("failed, rc=");
      Serial.print(mqtt_wifi.state());
      Serial.println(" try again in 5 seconds");
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



/* MQTT received message callback */
void mqttCallback(char* topic, byte* payload, unsigned int length) {

  msg_cmd = "";
  for (int i = 0; i < length; i++) {
    msg_cmd += (char)payload[i];
  }
  Serial.println("msg_cmd = " + String(msg_cmd));

  /* -- List file -- */
  if (msg_cmd == "ls") {

    Serial.println("cmd ls");
    state_list_file = true;
  }
  else {

    // -- Checking for other mode --
    String _delmode = "del:";
    for (int i = 0; i < msg_cmd.length(); i++) {
      if (msg_cmd.substring(i, i + _delmode.length()) == _delmode) {
        String _targetDelFile = msg_cmd.substring(i + _delmode.length(), msg_cmd.length());
        Serial.println("_targetDelFile = " + _targetDelFile);
        if (deleteFile(_targetDelFile.c_str())) {
          Serial.println("removed ok");
        } else {
          Serial.println("remove failed");
        }
        return;
      } else {
        //..
      }
    }

    String _dumpmode = "dump:";
    for (int i = 0; i < msg_cmd.length(); i++) {
      if (msg_cmd.substring(i, i + _dumpmode.length()) == _dumpmode) {
                String _targetDumpFile = msg_cmd.substring(i + _dumpmode.length(), msg_cmd.length());
                Serial.println("_targetDumpFile = " + _targetDumpFile);

       // String _targetDumpFile = spiff.readFile(SPIFFS, "/buffer.txt");
        dumpFile(_targetDumpFile);
        return;
      } else {
        //..
      }
    }

  }

}


void pushmessage(String _payload) {
  _payload.toCharArray(msg, MSG_BUFFER_SIZE);
  mqtt_wifi.publish(topics, msg);
}


boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(mqtt_broker);



  // Connect to MQTT Broker
  boolean status = mqtt_wifi.connect(mqtt_broker);
  status_network = status;

  // Or, if you want to authenticate MQTT:
  //boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

  if (status == false) {
    Serial.println(" fail");
    status_network = status;
    return false;
  }
  Serial.println(" success");
  String online = device_id + " Online";

  mqtt_wifi.publish(mqtt_pub.c_str(), online.c_str());
  mqtt_wifi.subscribe(mqtt_sub.c_str());
  return mqtt_wifi.connected();
}

#endif
