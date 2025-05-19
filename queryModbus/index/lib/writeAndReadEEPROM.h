#ifndef writeAndReadEEPROM_h
#define writeAndReadEEPROM_h

int writeCounter = 0;  // Contador en RAM

void writeStringToEEPROM(int addrOffset, const String& strToWrite) {
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  EEPROM.commit();
  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
    EEPROM.commit();
  }

  writeCounter++;  // Incrementamos el contador

  M5.Lcd.setTextColor(GREEN);
  String resp;
  StaticJsonDocument<300> doc;
  doc["status"] = "success";
  doc["message"] = "¡Operación completada con éxito!";
  doc["contador"] = writeCounter;  // Mostramos el contador
  serializeJson(doc, resp);
  // M5.Lcd.println(resp);
  M5.Lcd.println(doc["message"].as<const char*>());
  Serial.println(resp);
}

String readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}
#endif