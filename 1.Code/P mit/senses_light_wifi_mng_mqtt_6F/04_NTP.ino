unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}


String getTime_str()
{
  if (status_mqtt == true)
  {
    unsigned long t =  getTime();
    String t_str = String(t);

    //Serial.print(" A getTime_str  : "); Serial.println(t_str);

    EEPROM.writeString(add_unix_time, t_str);
    EEPROM.commit();

    return t_str;
  }

  else
  {
    String _unix_time = EEPROM.readString(add_unix_time);

    unsigned long _unix_time_long = (long) strtol(_unix_time.c_str(), NULL, 0);

    //Serial.print(" B getTime_str  : "); Serial.println(_unix_time_long);

    _unix_time_long += TIME_TO_SLEEP + time_operate_sec;

    String _unix_time_str = String(_unix_time_long);

    //Serial.print(" C getTime_str  : "); Serial.println(_unix_time_str);

    EEPROM.writeString(add_unix_time, _unix_time_str);
    EEPROM.commit();

    return  _unix_time_str;
  }
}
