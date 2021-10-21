void setup() {
  Serial.begin(9600);

  String mac_1 = "AABBCC";
  String mac_2 = "DDEEFF";

  String mac_1_str;
  mac_1_str =  x2i(str2charArray(mac_1));
  unsigned int mac_1_int = mac_1_str.toInt();

  String mac_2_str;
  mac_2_str =  x2i(str2charArray(mac_2));
  unsigned int mac_2_int = mac_2_str.toInt();


  char payload_mac[6];

  payload_mac[0] = (mac_1_int >> 16);
  payload_mac[1] = (mac_1_int >> 8);
  payload_mac[2] = (mac_1_int & 0xFF);
  payload_mac[3] = (mac_2_int >> 16);
  payload_mac[4] = (mac_2_int >> 8);
  payload_mac[5] = (mac_2_int & 0xFF);


  Serial.print("\nmac full       : "); Serial.println(mac_1 + mac_2);
  Serial.print("payload_mac[0] : "); Serial.println(payload_mac[0], HEX);
  Serial.print("payload_mac[1] : "); Serial.println(payload_mac[1], HEX);
  Serial.print("payload_mac[2] : "); Serial.println(payload_mac[2], HEX);
  Serial.print("payload_mac[3] : "); Serial.println(payload_mac[3], HEX);
  Serial.print("payload_mac[4] : "); Serial.println(payload_mac[4], HEX);
  Serial.print("payload_mac[5] : "); Serial.println(payload_mac[5], HEX);
  Serial.println("==========================");


  int myVal = -999;
  byte payload[2];
  payload[0] = highByte(myVal);
  payload[1] = lowByte(myVal);

  Serial.print("myVal : "); Serial.println(myVal);
  Serial.print("payload[0] highByte : "); Serial.println( payload[0], HEX);
  Serial.print("payload[1] lowByte  : "); Serial.println( payload[1], HEX);

  String payload_hex_1 = String(payload[0], HEX);
  String payload_hex_2 = String(payload[1], HEX);
  String payload_hex = payload_hex_1 + payload_hex_2;
  unsigned int payload_dec =  hexToDec(payload_hex);

  Serial.print("DEC                 : "); Serial.println(payload_dec);
  Serial.println("-------------------------");

  myVal = 999;
  payload[0] = highByte(myVal);
  payload[1] = lowByte(myVal);
  Serial.print("myVal : "); Serial.println(myVal);
  Serial.print("payload[0] highByte : "); Serial.println( payload[0], HEX);
  Serial.print("payload[1] lowByte  : "); Serial.println( payload[1], HEX);

  payload_hex_1 = String(payload[0], HEX);
  payload_hex_2 = String(payload[1], HEX);
  payload_hex = payload_hex_1 + payload_hex_2;
  payload_dec =  hexToDec(payload_hex);

  Serial.print("DEC                 : "); Serial.println(payload_dec);
  Serial.println("-------------------------");


  delay(3000);
  ESP.restart();
}

void loop() {
  // put your main code here, to run repeatedly:

}

char* str2charArray(String str) {

  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);


}

long x2i(char *s)
{
  long x = 0;
  for (;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
    }
    else break;
    s++;
  }

  return x;
}


unsigned int hexToDec(String hexString) {

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}
