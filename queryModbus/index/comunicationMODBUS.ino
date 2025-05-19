// Librerías necesarias
#include "lib/CRC.h"                 // Cálculo de CRC para tramas Modbus
#include "lib/tramasModbus.h"        // Tramas predefinidas para consultas Modbus
#include "network/packagesent.h"     // Envío de datos por red
#include <stdio.h>

// Variables globales
unsigned long previousmillis0 = 0;
const unsigned long interval1 = 1000;
String sensorReadings;
String tramas[34];                   // Arreglo de tramas a enviar
byte plots_Modbus[8];               // Trama de 8 bytes para Modbus
int wz = 3;
int a = 0, b = 0, numberqueries = 0;
String val2;
size_t received_byteModbus = 0;
String NameVariable;

// Función para consultar un medidor Modbus
void ConsultaMedidorModbus(String address) {
  M5.Lcd.print("Enviando data...");
  Serial.println("Enviando data...");  // Imprime la dirección del medidor a consultar
  Serial.println(address);  // Imprime la dirección del medidor a consultar
  unsigned long currentMillis = millis();

  do {
    // Construye la trama con dirección y variable
    String plot = address + VariablesModbus[a];
    String CRC_16 = CyclicRedundancyCheck(plot);
    tramas[a] = plot + CRC_16;

    // Convierte trama en bytes para enviar
    DecodeData_modbus(tramas[a]);
    enviardato_Modbus();              // Envía trama por Serial2

    val2 = serialCheck_Modbus();     // Lee respuesta del dispositivo

    // Verifica que la respuesta tenga longitud válida
    if (val2.length() == 18) {
      delay(60);
      val2 = val2.substring(6, 14);  // Extrae parte útil de la trama

      // Guarda respuesta para depuración
      myArrayPrueba[a] = "value: " + val2;

      // Conversión de Hex a binario por partes
      String binaryNumer0 = addLeadingZeros(hexToBin(val2.substring(0, 2)), 8);
      String binaryNumer1 = addLeadingZeros(hexToBin(val2.substring(2, 4)), 8);
      String binaryNumer2 = addLeadingZeros(hexToBin(val2.substring(4, 6)), 8);
      String binaryNumer3 = addLeadingZeros(hexToBin(val2.substring(6, 8)), 8);

      // Une las partes en un solo valor binario
      String Binaryvalue = binaryNumer0 + binaryNumer1 + binaryNumer2 + binaryNumer3;
      int sizeBinaryvalue = Binaryvalue.length();

      // Extrae exponente IEEE-754
      int exponent = convertir(Binaryvalue.substring(1, 9)) - 127;

      // Extrae mantisa
      String mantissa = (exponent <= -1) ? Binaryvalue.substring(9) : "1" + Binaryvalue.substring(9);
      String mantissaEntero = mantissa.substring(0, exponent + 1);
      String mantissaDecimal = mantissa.substring(exponent + 1);

      // Convierte a número decimal flotante
      float NumeroEntero = 0;
      float NumeroDecimal = 0;
      int ExponentEntero = exponent;
      int ExponentDecimal = -1;

      // Parte entera de la mantisa
      for (int i = 0; i < mantissaEntero.length(); i++) {
        int element = mantissaEntero[i] - '0';
        NumeroEntero += element * pow(2, ExponentEntero);
        ExponentEntero--;
      }

      // Parte decimal de la mantisa
      for (int i = 0; i < mantissaDecimal.length(); i++) {
        int element = mantissaDecimal[i] - '0';
        NumeroDecimal += element * pow(2, ExponentDecimal);
        ExponentDecimal--;
      }

      // Determina nombre de variable
      int posicion = a - 1;
      if (posicion == -1) {
        NameVariableModbus(tramas[34]);
      } else {
        NameVariableModbus(tramas[posicion]);
      }

      // Valor final en flotante
      float ValueTotal = NumeroEntero + NumeroDecimal;

      // Guarda variable con nombre y valor
      myArray_Modbus[a] = NameVar + ": " + String(ValueTotal);

      // Si se terminó de recorrer todas las variables
      if (a == 34) {
        M5.Lcd.begin();  // Inicializa pantalla
        M5.Lcd.print("Enviando data...");
        a = 0;
        numberqueries = 0;
        nElements--;
        posicion = 0;
        myArray_Modbus[a];
        myArray_Modbus[35] = Time;

        // Serializa array a JSON
        String jsonStringModbus = JSON.stringify(myArray_Modbus);

        // Envía datos al servidor según el medidor
        if (address == "03") {
          meterSerial = serialTresString;
          sensorReadings = httpPOSTReq(server, serverLan, path, jsonStringModbus, meterSerial);
          Serial.println("Sensor Readings com 03: ");
          Serial.println(sensorReadings);
          Serial.println("Medidor 03");
        }
        if (address == "02") {
          meterSerial = serialDosString;
          sensorReadings = httpPOSTReq(server, serverLan, path, jsonStringModbus, meterSerial);
          Serial.println("Sensor Readings com 02: ");
          Serial.println(sensorReadings);
          Serial.println("Medidor 02");
        }
        if (address == "01") {
          meterSerial = serialUnoString;
          sensorReadings = httpPOSTReq(server, serverLan, path, jsonStringModbus, meterSerial);
          Serial.println("Sensor Readings com 01: ");
          Serial.println(sensorReadings);
          Serial.println("Medidor 01");
        }

        b = 0;  // Reinicia contador de reintentos
      } else {
        a++;
        numberqueries++;
      }

    } else {
      // Si los datos recibidos no son válidos
      myArrayPruebab4[a] = "incorrect data, the query is sent again: " + String(tramas[a]);
      String jsonStrinCRC = JSON.stringify(myArrayPruebab4);
      b++;
      numberqueries++;
      delay(500);

      // Si se alcanzó el máximo de reintentos
      if (b == 10) {
        M5.Lcd.begin();
        String jsonError = "";
        numberqueries = 0;
        b = 0;

        // Construye mensaje de error
        StaticJsonDocument<300> doc;
        doc["message"] = "exceeded the maximum number of attempts";
        doc["Time"] = Time.substring(0, 24);
        serializeJson(doc, jsonError);
        delay(1000);

        // Envía error al servidor correspondiente
        if (address == "01") {
          meterSerial = serialUnoString;
          sensorReadings = httpPOSTReq(server, serverLan, errorPath, jsonError, meterSerial);
          M5.lcd.print(sensorReadings);
          delay(5000);
        }
        if (address == "02") {
          meterSerial = serialDosString;
          sensorReadings = httpPOSTReq(server, serverLan, errorPath, jsonError, meterSerial);
          delay(1000);
          Serial.println(sensorReadings);
        }
        if (address == "03") {
          meterSerial = serialTresString;
          sensorReadings = httpPOSTReq(server, serverLan, errorPath, jsonError, meterSerial);
          delay(1000);
          Serial.println(sensorReadings);
        }
      }
    }

    delay(600);  // Espera antes de la siguiente iteración
    esp_task_wdt_reset();  // Resetea el watchdog
  } while (numberqueries > 0);  // Se repite si hubo reintentos

  return;
}

// Rellena con ceros a la izquierda hasta alcanzar el largo deseado
String addLeadingZeros(String binaryNumber, int desiredLength) {
  int currentLength = binaryNumber.length();
  if (currentLength >= desiredLength) return binaryNumber;

  int numZeros = desiredLength - currentLength;
  for (int i = 0; i < numZeros; i++) {
    binaryNumber = "0" + binaryNumber;
  }
  return binaryNumber;
}

// Convierte una cadena binaria a decimal
int convertir(String binStr) {
  int val = 0;
  for (int i = 0; i < binStr.length(); i++) {
    val = val * 2 + (binStr[i] - 48);
  }
  return val;
}

// Convierte una cadena hexadecimal a arreglo de bytes
void DecodeData_modbus(String datos) {
  const int numValues = 8;
  for (int i = 0; i < numValues; i++) {
    String valueStr = datos.substring(i * 2, i * 2 + 2);
    plots_Modbus[i] = strtoul(valueStr.c_str(), nullptr, 16);
  }
}

// Envía la trama Modbus por el puerto serial
void enviardato_Modbus() {
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  Serial2.write(plots_Modbus, 8);
}

// Lee la respuesta Modbus del puerto serial y la convierte a hex
String serialCheck_Modbus() {
  String x;
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  int received_byteModbus = Serial2.available();
  byte BufferValue[received_byteModbus];

  for (int i = 0; i < received_byteModbus; i++) {
    BufferValue[i] = Serial2.read();
  }

  for (int i = 0; i < received_byteModbus; i++) {
    x += decToHex(BufferValue[i], 2);
  }
  return x;
}

// Convierte hexadecimal a binario
String hexToBin(String hexString) {
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

  String NumberBinary = String(decValue, BIN);
  return NumberBinary;
}
