#include <M5Stack.h>
#include <esp_task_wdt.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <stdlib.h>
#include <NTPClient.h>
#include <SPI.h>
#include <EthernetLarge.h>
#include <EthernetUdp.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "EEPROM.h"
#include "lib/defines.h"
#include "image/xbm.h"
#include "image/loginIndex.h"
#include "image/updateIndex.h"
#include "lib/writeAndReadEEPROM.h"
#include "CronAlarms.h"

#include <Preferences.h>
// Preferences prefs;

TaskHandle_t Task1;
CronId id;

JSONVar myArrayPruebab3, myArrayPruebab4, myArrayPruebab2;
JSONVar myArray_Modbus;
JSONVar myArrayPrueba;

int parity = 1;
int initialsize = 0;

WiFiClientSecure client0;
EthernetClient client1;

WiFiUDP ntpUDP;
EthernetUDP Udp;

NTPClient timeClient1(Udp, "0.pool.ntp.org", 36000, 60000);
NTPClient timeClient0(ntpUDP, "0.pool.ntp.org", 36000, 60000);

WebServer serverUpdate(80);

// volatile size_t sent_bytes = 0, received_byte = 0;
// unsigned long int epochTimeFromServer = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;
unsigned long previousMillis_test = 0;
const unsigned long interval_test = 60000;
time_t epoch;

// Función: Aumentar el tiempo del watchdog para evitar reinicios por tareas largas
void increaseWatchdogTime(){
  esp_task_wdt_init(10, true); // Aumentar el tiempo del watchdog a 10 segundos
}

// [FUNCIÓN PRINCIPAL] Consulta o registra medidores según el estado del comunicador en la EEPROM
void queryMeter(){
  M5.lcd.clear();         // [M5STACK] Limpia la pantalla LCD
  M5.Lcd.setCursor(0, 0); // [M5STACK] Posiciona el cursor en la pantalla

  int numeroMeter = readStringFromEEPROM(20).toInt(); // [EEPROM] Lee la cantidad de medidores almacenada

  if (registeredMedidor == "true"){
    // Mostrar estado registrado
    M5.Lcd.print("Estado de registro: ");
    M5.Lcd.println(registeredMedidor);
    delay(2000);

    // Consultar múltiples medidores si existen
    if (numeroMeter >= 1) {
      for (int i = 0; i < numeroMeter; i++) {
        String meterID = numberMeters[i]; // ID del medidor actual
        
        Serial.print("Medidor ID: ");
        Serial.println(meterID);

        // Serial.print("Índice en arreglo: ");
        // Serial.println(i);

        Serial.println("Consultando medidor...");
        ConsultaMedidorModbus(meterID); // Realiza la consulta Modbus

        delay(100); // Espera para evitar saturación del bus
      }
    }

  } else {
    // Comienza el proceso de registro
    M5.Lcd.print("Estado de registro: ");
    M5.Lcd.println(registeredMedidor);
    delay(1000);

    Serial.println("Config red #1");
    configureRed(); // [RED/WIFI] Configura la red (posiblemente WiFi)
    M5.Lcd.println("Enviando registro...1");
    queryDBMetter();
  }
}

// Verificar y registrar medidor
void queryDBMetter(){
  if (registeredMedidor == "false") {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Medidor no registrado");
    Serial.println("Medidor no registrado");
    M5.Lcd.println("Revisando registro...");
    Serial.println("Revisando registro...");
    delay(1000);

    if (idCommunicator == 0) {
      bool statusProjectMeter = false;

      String seriales[3] = {serialUnoString, serialDosString, serialTresString};
      String direcciones[3] = {"01", "02", "03"};

      for (int i = 0; i < 3; i++) {
        if (numberMeters[i] == direcciones[i]) {
          String result = queryDBSerial(seriales[i]);

          if (result == "true") {
            M5.Lcd.print("Medidor Registrado en DB: ");
            M5.Lcd.println(seriales[i]);
            Serial.print("Medidor Registrado en DB: ");
            Serial.println(seriales[i]);
            statusProjectMeter = true;
          } else if (result == "false") {
            M5.Lcd.print("Medidor NO Registrado: ");
            M5.Lcd.println(seriales[i]);
            Serial.print("Medidor NO Registrado: ");
            Serial.println(seriales[i]);
            M5.Lcd.println("Enviando registro >> DB");
            Serial.println("Enviando registro >> DB");
            if (EEPROM.read(123) == 0){
              // Registro de Comunicador
              sendRegiterdCommunicator(seriales[i]); // Descomenta si deseas registrar aquí
              delay(100);
              idCommunicator = readStringFromEEPROM(123).toInt();
              M5.Lcd.print("Communicator: ");
              M5.Lcd.println(idCommunicator);
            }
            // Registro de medidor
            sendRegisterdMeter(seriales[i]);
          } else { // "null"
            M5.Lcd.print("Error consultando serial: ");
            M5.Lcd.println(seriales[i]);
            Serial.print("Error consultando serial: ");
            Serial.println(seriales[i]);
          }

          delay(500);
        }
      }

      if (statusProjectMeter) {
        registeredMedidor = "true";
        Serial.print("Registro del comunicador: ");
        Serial.println(registeredMedidor);
        writeStringToEEPROM(200, registeredMedidor);
      }
    }
  }
}

// Borrar EEPROM
void BorrEeprom(){
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.println("<< Borrando datos >>");

  for (int i = 0; i < EEPROM.length(); i++){
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
  M5.Lcd.println("<< Datos borrados >>");
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.println("<<Reiniciando Comunicad>>");

  delay(1000);
  ESP.restart();
}

// Reinicia el sistema
void reset(){
  // bot.sendMessage(CHAT_ID, "reset system", "");  // (Opcional) Enviar mensaje de reinicio por bot de Telegram
  Serial.print("Reinicio por Reset()");
  ESP.restart(); // Comando para reiniciar el microcontrolador (ESP32/ESP8266)
}

//Conigura y sincroniza hora
void ConfigSincroRelog(){
  Serial.print("Entrando a conf Relog");
  bool actualizado = false;
  epoch = 0; // Aseguramos que no se use un valor anterior

  M5.Lcd.println("Cargando dato hora");
  Serial.println("Cargando datos de hora");

  if (modo == "1") {
    timeClient0.begin();
    timeClient0.setTimeOffset(-18000);
    actualizado = timeClient0.update();
    if (actualizado) {
      epoch = timeClient0.getEpochTime();
    }
  } else {
    timeClient1.begin();
    timeClient1.setTimeOffset(-18000);
    actualizado = timeClient1.update();
    if (actualizado) {
      epoch = timeClient1.getEpochTime();
    }
  }

  if (actualizado && epoch > 1000000000) {  // Hora válida (> 2001)
    struct tm *ptm = gmtime((time_t *)&epoch);
    timeval tv = {mktime(ptm), 0};
    settimeofday(&tv, nullptr);
    Serial.println("Hora sincronizada correctamente");
    M5.Lcd.println("Hora actualizada");
  } else {
    Serial.println("Fallo al obtener hora desde NTP");
    M5.Lcd.println("Fallo NTP");

    // if (registeredMedidor == "false") {
      Serial.println("Reinicio por falta de hora");
      M5.Lcd.println("Reinicio por falta de hora");
      delay(1000);
      reset(); // o ESP.restart();
    // } else {
    //   Serial.println("Continúa sin hora, medidor ya registrado");
    //   // Opcional: mantener hora previa si existía
    // }
  }
}

// [SETUP INICIAL] Inicialización del sistema
void setup() {
  // ==== INICIALIZACIÓN BÁSICA ====
  macAddr = WiFi.macAddress();  // MAC Devuelve algo como "24:0A:C4:12:34:56"
  Serial.print("MAC Comunicador: ");
  Serial.println(macAddr);
  macAddr.replace(":","");

  M5.begin();
  M5.Power.begin();
  while (!Serial);  // Esperar al puerto serial

  M5.lcd.setTextSize(2);
  M5.lcd.clear();
  M5.Lcd.setCursor(0, 0);
  delay(1000);

  // ==== CONFIGURACIÓN SERIAL2 ====
  if (parity == 1) {
    Serial2.begin(9600);
  } else {
    Serial2.begin(BAUD, SERIAL_8E1, RXPIN, TXPIN);
  }

  // ==== INICIALIZAR EEPROM ====
  if (!EEPROM.begin(SIZE)) {
    M5.Lcd.println("\n¡Fallo al iniciar EEPROM!");
    delay(10000);
  }

  // Mostrar contenido EEPROM si no está vacía
  if (EEPROM.read(249) != 255) {
    M5.Lcd.println("\nDatos en EEPROM:");
    for (int i = 0; i < SIZE; i++) {
      M5.Lcd.printf("%d ", EEPROM.read(i));
    }
  } else {
    // Limpiar EEPROM si está vacía
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
  }
  delay(3000);

  // ==== BORRAR EEPROM SI SE PRESIONA BOTÓN C ====
  if (M5.BtnC.isPressed()) {
    BorrEeprom();
  }

  // ==== LEER ESTADO DE REGISTRO ====
  if (EEPROM.read(200) > 0) {
    registeredMedidor = readStringFromEEPROM(200);
  } else {
    registeredMedidor = "false";
  }

  // ==== LÓGICA DE ARRANQUE SEGÚN REGISTRO ====
  if (registeredMedidor == "true" || EEPROM.read(0) > 0) {
    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Estado de registro: ");
    M5.Lcd.println(registeredMedidor);
    delay(1500);

    // === LEER ID DE PROYECTO ===
    if (EEPROM.read(0) > 0) {
      projectId = readStringFromEEPROM(0).toInt();
      M5.Lcd.print("Proyecto: ");
      M5.Lcd.println(projectId);
    }

    // === LEER ID DEL COMUNICADOR ===
    if (EEPROM.read(123) > 0) {
      idCommunicator = readStringFromEEPROM(123).toInt();
      M5.Lcd.print("Communicator: ");
      M5.Lcd.println(idCommunicator);
    } else {
      M5.Lcd.println("Communicator no reg.");
      // M5.Lcd.println("Enviando...");
      // sendRegiterdCommunicator(deviceSerial);  // ← activar si quieres registrar aquí
      // idCommunicator = readStringFromEEPROM(123);  // Releer después del registro
    }

    // === LEER MODO DE CONEXIÓN ===
    if (EEPROM.read(120) > 0) {
      modo = readStringFromEEPROM(120);
      M5.Lcd.print("Modo Red: ");
      M5.Lcd.println(modo == "1" ? "WiFi" : "Ethernet");
    }

    // === LEER NÚMERO DE MEDIDORES ===
    if (EEPROM.read(20) > 0) {
      String dato = readStringFromEEPROM(20);
      if (dato == "3") {
        numberMeters[0] = "01"; numberMeters[1] = "02"; numberMeters[2] = "03";
      } else if (dato == "2") {
        numberMeters[0] = "01"; numberMeters[1] = "02";
      } else {
        numberMeters[0] = "01";
      }
      Dirstring = dato;
    }

    // === LEER SERIALES DE MEDIDORES ===
    StaticJsonDocument<300> seriales;
    if (EEPROM.read(30) > 0) {
      serialUnoString = readStringFromEEPROM(30);
      seriales["medidorUno"] = serialUnoString;
    }
    if (EEPROM.read(40) > 0) {
      serialDosString = readStringFromEEPROM(40);
      seriales["medidorDos"] = serialDosString;
    }
    if (EEPROM.read(50) > 0) {
      serialTresString = readStringFromEEPROM(50);
      seriales["medidorTres"] = serialTresString;
    }

    String SerialesMedidor;
    serializeJson(seriales, SerialesMedidor);
    M5.Lcd.println("Serial Medidor:");
    M5.Lcd.println(SerialesMedidor);

    // === CONFIGURAR RED ===
    if (modo == "1") {  // WiFi
      if (EEPROM.read(70) > 0) ssidString = readStringFromEEPROM(70);
      if (EEPROM.read(90) > 0) passwordString = readStringFromEEPROM(90);
    } else {  // Ethernet
      if (EEPROM.read(130) > 0) {
        macbystring = readStringFromEEPROM(130);
        M5.Lcd.print("MAC: ");
        M5.Lcd.println(macbystring);
      }
    }

    Serial.println("Configurando Red #2");
    configureRed();

    // === CONSULTA Y REGISTRO EN DB SI FALTA ===
    queryDBMetter();
  }

  // ==== PRIMER INICIO (SIN DATOS EEPROM) ====
  else if (registeredMedidor == "false" && EEPROM.read(0) == 0) {
    do {
      M5.lcd.clear();
      M5.Lcd.setCursor(0, 0);
      Serial.println("Configurando Red #3");
      configureRed();
    } while (status > 0);

    M5.lcd.clear();
    M5.Lcd.println("Comunicador no registrado");
    M5.Lcd.println("Enviando registro...");
    delay(3000);

    queryDBMetter();
    delay(3000);
  }

  // ==== SINCRONIZAR RELOJ ====
  ConfigSincroRelog();

  // ==== CONFIGURAR TAREAS PROGRAMADAS ====
  if (parity == 1) {
    Cron.create("0 15 0 * * *", reset, false);        // Reinicio diario a las 00:15
    Cron.create("0 */5 * * * *", queryMeter, false);  // Lectura cada 5 min
  }

  // ==== CONFIGURAR PINES RS-485 ====
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);

  M5.Lcd.println("Setup completo OK");
  Serial.println("Setup completo OK");
  
  M5.Lcd.begin();  // (opcional si ya se inicializó M5.begin())
  delay(3000);
}

// [LOOP PRINCIPAL] proceso constante despues del setup
void loop() {
  M5.update(); // Actualiza botones físicos
  M5.Lcd.setTextColor(WHITE);
  serverUpdate.handleClient(); // Servidor web activo

  unsigned long currentMillis = millis();

  // Tareas temporizadas
  if (currentMillis - previousMillis >= interval) {
#ifdef __AVR__ 
system_tick(); // llamado necesario para AVR a 1Hz
#endif

    time_t tnow = time(nullptr);
    Time = String(asctime(gmtime(&tnow)));

    // Actualiza Pantalla cada 5 minutos
    if (minutos != Time.substring(15, 16)) {
      minutos = Time.substring(15, 16);
      M5.Lcd.fillScreen(0);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.println("Horus Smart Energy");

      // Mostrar fecha actual
      String fecha = Time.substring(0, 16);
      String year = Time.substring(20);
      fecha = fecha + " : " + year;
      M5.Lcd.drawString(fecha, 0, 70, 1);

      // Indicadores de red
      if (modo == "1") {
        // conexion WiFi
        if (WiFi.status() != WL_CONNECTED) {
          M5.Lcd.print("Estado red Wifi: ");
          M5.Lcd.setTextColor(RED);
          M5.Lcd.println("Desconectado");
          // M5.Lcd.drawCircle(220, 25, 7, WHITE);
          // M5.Lcd.fillCircle(220, 25, 6, RED);
          WiFi.disconnect();
          WiFi.reconnect();
        } else {
          M5.Lcd.print("Estado red WiFi: ");
          M5.Lcd.setTextColor(GREEN);
          M5.Lcd.println("Conectado");
          // M5.Lcd.drawCircle(6, 30, 7, WHITE);
          // M5.Lcd.fillCircle(6, 30, 6, GREEN);
        }
      } else {
        // conexion Ethernet
        if (Ethernet.linkStatus() == Unknown)
        {
          M5.Lcd.println("Ethernet status unknown");
        }
        else if (Ethernet.linkStatus() == LinkON)
        {
          M5.Lcd.print("Ethernet: ");
          M5.Lcd.setTextColor(GREEN);
          M5.Lcd.println("Conectado");
          // M5.Lcd.drawCircle(6, 30, 7, WHITE);
          // M5.Lcd.fillCircle(6, 30, 6, GREEN);
        }
        else if (Ethernet.linkStatus() == LinkOFF)
        {
          M5.Lcd.print("Ethernet: ");
          M5.Lcd.setTextColor(RED);
          M5.Lcd.println("Desconectado");
          // M5.Lcd.drawCircle(245, 25, 7, WHITE);
          // M5.Lcd.fillCircle(245, 25, 6, RED);
        }
      }

      // Si o No - Muestreo de seriales en pantalla principal
      if (serialUnoString != "") {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("SN COM 1: ");
        M5.Lcd.println(serialUnoString);
      }
      else
      {
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println("No medidores conectados ");
      }

      if (registeredMedidor != "false") {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("Estado de registro: ");
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.println(registeredMedidor);
      } else {
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.print("Estado de registro: ");
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println(registeredMedidor);
      }

      // inserta logo en pantalla principal
      M5.Lcd.drawXBitmap(axisx, axisy, logo, logoWidth, logoHeight, TFT_WHITE, TFT_BLACK);
    }

    Cron.delay(); // Procesar tareas programadas
    previousMillis = currentMillis;
  }

  esp_task_wdt_reset(); // Evitar reinicio por watchdog
}
