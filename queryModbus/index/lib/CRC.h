#ifndef CRC_h
#define CRC_h

// Convierte una cadena hexadecimal de 2 caracteres ("AF") en un byte (0-255)
byte StrtoByte(String strHex) {
  char charArray[3];  // Arreglo de 3 caracteres para almacenar "AF" + '\0'
  strHex.toCharArray(charArray, 3);  // Copia el contenido del String a char[]
  return strtoul(charArray, NULL, 16);  // Convierte el string en número base 16
}

// Calcula el CRC-16 MODBUS de una trama hexadecimal
String CyclicRedundancyCheck(String hexFrame) {
  int numBytes = hexFrame.length() / 2;
  byte byteArray[numBytes];  // Arreglo donde guardamos los bytes convertidos

  // Convertimos el string hexadecimal a arreglo de bytes reales
  for (int i = 0; i < numBytes; i++) {
    String hexByte = hexFrame.substring(2 * i, 2 * i + 2);  // Extrae 2 caracteres
    byteArray[i] = StrtoByte(hexByte);  // Convierte "AF" → 0xAF
  }

  // Iniciamos el CRC con el valor estándar para Modbus
  uint16_t crc = 0xFFFF;

  // Algoritmo CRC-16 Modbus estándar
  for (int pos = 0; pos < numBytes; pos++) {
    crc ^= byteArray[pos];  // XOR del byte actual con el CRC

    // Recorremos cada uno de los 8 bits del byte
    for (int i = 0; i < 8; i++) {
      if (crc & 0x0001) {  // Si el bit menos significativo es 1
        crc >>= 1;         // Desplazamos a la derecha
        crc ^= 0xA001;     // Aplicamos el polinomio estándar de Modbus
      } else {
        crc >>= 1;  // Si no, solo desplazamos a la derecha
      }
    }
  }

  // Convertimos el CRC final a String hexadecimal (por ejemplo: "4C27")
  String crcHex = String(crc, HEX);
  crcHex.toUpperCase();  // Lo convertimos a mayúsculas

  // Aseguramos que el string tenga siempre 4 caracteres
  while (crcHex.length() < 4) {
    crcHex = "0" + crcHex;
  }

  // Intercambiamos los bytes (Modbus requiere little-endian: LSB primero)
  String crcSwapped = crcHex.substring(2, 4) + crcHex.substring(0, 2);

  return crcSwapped;  // Devuelve algo como "274C"
}

#endif