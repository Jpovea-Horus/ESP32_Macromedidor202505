// configuaciones menú inicial
#include "lib/converters.h"
#include "lib/writeAndReadEEPROM.h"

int datoMenuInicial = 0;
int eepromsiza = 0;
int item;
String key;

void menu(int dato) {
  item = dato;
  switch (item) {
    case 0:
      M5.Lcd.setTextColor(YELLOW);
      M5.Lcd.print("Ingrese codigo de proyecto");
      Serial.print("#Ingrese codigo del proyecto: ");
      key = frase();
      projectId = key.toInt();
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.println(projectId);
      Serial.println(projectId);
      if (EEPROM.read(0) == 0) {
        writeStringToEEPROM(0, key);
      }
      break;
    case 1: 
      M5.Lcd.setTextColor(YELLOW);
      M5.Lcd.println("Ingrese la cant. de medid: ");
      Serial.print("#Ingrese la cantidad de medidores: ");
      key = frase();
      key = limpiarString(key);
      Serial.println(key);
      writeStringToEEPROM(20, key);
      // Serial.println(key);
      
      if (EEPROM.read(20) > 0) {        
        String dato = readStringFromEEPROM(20);
        dato.trim();
        Serial.println("Valor procesado: [" + dato + "]");
        if (dato == "3") {
          numberMeters[0] = "01";
          numberMeters[1] = "02";
          numberMeters[2] = "03";
          M5.Lcd.setTextColor(WHITE);
          M5.Lcd.println("Direccion de medidores: ");
          Serial.print("##Direccion de medidores: ");
          M5.Lcd.setTextColor(GREEN);
          M5.Lcd.print(numberMeters[0]);
          Serial.print(numberMeters[0]);
          M5.Lcd.print(", ");
          Serial.println(", ");
          M5.Lcd.println(numberMeters[1]);
          Serial.println(numberMeters[1]);
          M5.Lcd.print(", ");
          Serial.println(", ");
          M5.Lcd.print(numberMeters[2]);
          Serial.println(numberMeters[2]);
          M5.Lcd.setTextColor(YELLOW);
        } else if (dato == "2") {
          numberMeters[0] = "01";
          numberMeters[1] = "02";
          M5.Lcd.setTextColor(WHITE);
          M5.Lcd.println("Direccion de medidores: ");
          Serial.print("##Direccion de medidores: ");
          M5.Lcd.setTextColor(GREEN);
          M5.Lcd.print(numberMeters[0]);
          Serial.print(numberMeters[0]);
          M5.Lcd.print(", ");
          Serial.println(", ");
          M5.Lcd.println(numberMeters[1]);
          Serial.println(numberMeters[1]);
          M5.Lcd.setTextColor(YELLOW);
        } else if (dato == "1") {
          numberMeters[0] = "01";
          M5.Lcd.setTextColor(WHITE);
          M5.Lcd.print("Direccion de medidores: ");
          Serial.print("##Direccion de medidores: ");
          M5.Lcd.setTextColor(GREEN);
          M5.Lcd.println(numberMeters[0]);
          Serial.println(numberMeters[0]);
          M5.Lcd.setTextColor(YELLOW);
        }
        delay(3000);
      }
      break;
    // case 2: // Actualizado a memoria preferences.h
    //   Serial.println("Inicio conexion WiFi 1");
    //   M5.Lcd.setTextColor(WHITE);
    //   M5.Lcd.println("##SSID de tu red: ");
    //   Serial.print("##SSID de tu red: ");
    //   key = frase();
    //   ssidString = key;
    //   ssid = converterStrcChar(key);
    //   M5.Lcd.setTextColor(GREEN);
    //   M5.Lcd.println(ssidString);
    //   Serial.println(ssidString);
    //   writeStringToEEPROM(70, key);
    //   eepromsiza = EEPROM.read(70);
    //   if (password == "") {
    //     M5.Lcd.setTextColor(WHITE);
    //     M5.Lcd.print("##password: ");
    //     Serial.print("##password: ");
    //     key = frase();
    //     passwordString = key;
    //     password = converterStrcChar(key);
    //     M5.Lcd.setTextColor(GREEN);
    //     M5.Lcd.println("*************");
    //     Serial.println("*************");
    //     if (EEPROM.read(90) == 0) {
    //       writeStringToEEPROM(90, key);
    //     }
    //   }
    //   delay(5000);
    //   break;
    case 2:
      Serial.println("Inicio conexion WiFi 1");
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.println("##SSID de tu red: ");
      Serial.print("##SSID de tu red: ");
    
      key = frase(); // <- función: captura usuario
      key = limpiarString(key);
      writeStringToEEPROM(70, key);
      ssidString = key;
      ssid = converterStrcChar(key); // <- convierte String a const char*
      
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.println(ssidString);
      Serial.println(ssidString);
    
      // Guardar SSID en Preferences
      prefs.begin("wifi", false);  // false = modo escritura
      prefs.putString("ssid", ssidString);
    
      if (password == "") {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("##password: ");
        Serial.print("##password: ");
    
        key = frase(); // función: captura contraseña
        key = limpiarString(key);
        writeStringToEEPROM(90, key);
        passwordString = key;
        password = converterStrcChar(key);
    
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.println("*************");
        Serial.println("*************");
    
        prefs.putString("pass", passwordString); // Guardar contraseña
      }
  
    prefs.end(); // Finalizar uso de Preferences
  
    delay(5000);
    break;
  }
}

void menuinicial() {
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println("1. Red WiFi");
  Serial.println("1. Red WiFi");
  M5.Lcd.drawCircle(155, 7, 7, WHITE);
  M5.Lcd.println("2. Red LAN");
  Serial.println("2. Red LAN");
  M5.Lcd.drawCircle(155, 24, 7, WHITE);
  M5.Lcd.println("3. Borrar EEPROM");
  Serial.println("3. Borrar EEPROM");
  M5.Lcd.drawCircle(210, 39, 7, WHITE);
  

  M5.Lcd.drawXBitmap(axisx, axisy, logo, logoWidth, logoHeight, TFT_WHITE, TFT_BLACK);
  datoMenuInicial = valor();
  switch (datoMenuInicial) {
    case 1: //menu inicial en 1 Red WiFi
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);

      M5.Lcd.println("1. Red WiFi: ");
      Serial.println("1. Red WiFi: #");
      M5.Lcd.drawCircle(155, 7, 7, WHITE);
      M5.Lcd.fillCircle(155, 7, 6, GREEN);
      // M5.Lcd.println("2. Red LAN");
      // Serial.println("2. Red LAN: ");

      modo = "1";
      writeStringToEEPROM(120, modo);
      delay(1000);
      break;
    case 2: //menu inicial en 2 Red Ethernet
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);
      // M5.Lcd.println("1. Red WiFi: ");
      // Serial.println("1. Red WiFi: ");
      M5.Lcd.println("2. Red LAN");
      Serial.println("2. Red LAN: #");
      M5.Lcd.drawCircle(155, 7, 7, WHITE);
      M5.Lcd.fillCircle(155, 7, 6, GREEN);
      str2mac(str, mac);
      key = str;
      Serial.println(str);
      delay(1000);
      LAN = 1;
      modo = "0";
      writeStringToEEPROM(120, modo);
      writeStringToEEPROM(130, key);
      break;
    case 3:
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);
      // M5.Lcd.println("1. Red WiFi: ");
      // Serial.println("1. Red WiFi: ";
      // M5.Lcd.println("2. Red LAN");
      // Serial.println("2. Red LAN");
      M5.Lcd.println("3. Borrar EEPROM: #");
      Serial.println("3. Borrar EEPROM: #");
      M5.Lcd.drawCircle(220, 7, 6, WHITE);    
      M5.Lcd.fillCircle(220, 7, 6, GREEN);
      delay(1000);
      modo = "";

      //Si se presiona 3;
      // Borrar EEPROM
      M5.Lcd.clear();
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.println("Borrando datos...");
    
        for (int i = 0; i < EEPROM.length(); i++) {
          EEPROM.write(i, 0);
        }
        EEPROM.commit();
    
      // Borrar datos de Preferences
      Preferences prefs;
      prefs.begin("wifi", false); // modo escritura
      prefs.remove("ssid");
      prefs.remove("pass");
      prefs.end();
    
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.println("Datos borrados");
      M5.Lcd.setTextColor(YELLOW);
      M5.Lcd.println("<< Favor Reiniciar Comunicador");
  }
}

int valor() { //menu inicial Lectura del dato serial para config
  int x = 0;
  String inString = "";

  while (true) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (isDigit(inChar)) {
        inString += inChar;
      }
    }

    if (!inString.isEmpty()) {
      x = inString.toInt();
      break;
    }
  }

  return x;
}

String limpiarString(String entrada) {
  entrada.trim();  // Elimina espacios al inicio y al final, incluyendo \n y \r
  return entrada;
}

String frase() { //Lectura del dato serial para config valores= Num Proy y Cant Medidores
  String inString = "";
  // Limpiar el buffer del puerto serie antes de leer la entrada
  while (Serial.available() > 0) {
    Serial.read();
  }

  while (true) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      inString += inChar;
    }
    if (!inString.isEmpty()) {
      break;
    }
  }
  return inString;
}
