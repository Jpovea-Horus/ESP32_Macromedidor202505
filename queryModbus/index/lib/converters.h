#ifndef converters_h
#define converters_h

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

String decToHex(byte decValue, byte desiredStringLength) {
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  return hexString;
}

char* converterStrcChar(String key) {
  int datoString_len = key.length() + 1;
  char* charArray = new char[datoString_len];
  key.toCharArray(charArray, datoString_len);
  // Serial.println(charArray);
  return charArray;
}

void str2mac(const char* str, uint8_t* mac) {
  char buffer[20];
  strcpy(buffer, str);
  const char* delims = ":";
  char* tok = strtok(buffer, delims);
  for (int i = 5; i >= 0; i--) {
    mac[i] = strtol(tok, NULL, 16);
    tok = strtok(NULL, delims);
  }
}

#endif