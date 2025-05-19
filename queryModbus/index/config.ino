#include "lib/converters.h"
#include <WiFi.h>
#include <Preferences.h>

Preferences prefs;

unsigned long previousMillisOne = 0;
const long intervalOne = 10000;

void configureRed() {
  if (modo == "") {
    M5.lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("configuracion inicial");
    Serial.println("configuracion inicial");
    delay(5000);
    M5.lcd.clear();
    menuinicial();
  }

  if (modo == "1") { // Conexion WiFi
    if (ssidString == "" && passwordString == "") {
      M5.lcd.clear();
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.println("configura una red wifi");
      Serial.println("configura una red wifi");
      delay(5000);
      menu(2);
    } else {
      Serial.println("Inicio conexion WiFi 2");
      ssid = converterStrcChar(ssidString);
      Serial.print("User: ");
      Serial.println(ssid);
      password = converterStrcChar(passwordString);
      Serial.println("Pass: ******");
      // Serial.println(passwordString); // SOLO PRUEBAS MUESTRA CONTRASEÑA WIFI
    }
  } else if (modo == "0") { // Conexion Ethernet
      if (macbystring == "") {
        M5.lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("se configura en la red LAN");
        delay(3000);
      } else {
        str2mac(str, mac);
        LAN = 1;
      }
    }

  if (ssidString != "" && passwordString != "") {
    int contador = 1;
    M5.lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("connecting to wifi");
    Serial.println("connecting to wifi");
    M5.Lcd.setTextColor(YELLOW);
    // Serial.println(ssidString); // SOLO PRUEBAS MUESTRA CONTRASEÑA WIFI
    // Serial.println(passwordString); // SOLO PRUEBAS MUESTRA CONTRASEÑA WIFI
    M5.Lcd.println(ssid);
    M5.Lcd.println("*********");
    delay(2000);
    bool statusConect = conexion();
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      M5.Lcd.setTextColor(RED);
      // Serial.println(WiFi.waitForConnectResult());
      M5.Lcd.println("WiFi Failed! ");
      Serial.println("WiFi Failed!");
      M5.Lcd.println("verifique:");
      M5.Lcd.println("SSD y Password");
      Serial.println("verifique el SSD y Password");
      delay(5000);
      M5.lcd.clear();
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);
      menuinicial();
    } else {
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.println("conectando... ");
      Serial.println("conectando... ");
      // M5.Lcd.setTextColor(GREEN);
      // Serial.println(ssid);
      // M5.Lcd.println(ssid);
      delay(1000);
    }
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print("ip adress: ");
    Serial.println("ip adress: ");
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.println(WiFi.localIP());
    Serial.println(WiFi.localIP());
    delay(6000);
    config();

  } else if (mac1 != 0 && LAN == 1) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Inicia Ethernet por DHCP");
    Serial.println("Iniciando conexion Ethernet...");

    // Inicializa SPI y Ethernet
    SPI.begin(SCK, MISO, MOSI, -1);
    Ethernet.init(CS);

    // Intentar DHCP
    if (Ethernet.begin(mac1) == 0) {
      M5.Lcd.println("Fallo DHCP, usando IP estática");
      M5.Lcd.println("usando IP estática");
      Serial.println("Error al configurar Ethernet mediante DHCP");
      // Ethernet.begin(mac1, ip); // intento con IP fija
    }
    
    // Verificar hardware
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("No se detectó hardware Ethernet.");
      M5.Lcd.println("HW Ethernet no detectado");
      if (registeredMedidor == "false"){
        Serial.println("Borrado en EthernetNoHardware");
        borrarIPyReiniciar();
      }
      return;
    }

    // Verificar cable
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("El cable Ethernet no está conectado.");
      M5.Lcd.println("Cable desconectado");
      esp_task_wdt_reset(); // Evitar reinicio por watchdog
      if (registeredMedidor == "false"){
        Serial.println("Borrado en LinkOFF");
        borrarIPyReiniciar();
      }
      return;
    }

    // conexion exitosa
    M5.Lcd.println("conexion LAN IP:");
    M5.Lcd.println(Ethernet.localIP());
    Serial.print("IP local: ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet: ");
    Serial.println(Ethernet.subnetMask());

    Udp.begin(Ethernet.localIP());

    delay(6000);
    config();
  }
}

void config() {
  // Serial.println("Configurando ");
  if (projectId == 0) {
    M5.lcd.clear();
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("codigo de projecto");
    Serial.println("codigo de projecto");
    delay(2000);
    menu(0);
    if (nElements == 0) {
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.println("medidores a instalar");
      delay(2000);
      menu(1);
    } else {
      String dato = readStringFromEEPROM(20);
      if(dato == "3"){
        numberMeters[0] = "01";
        numberMeters[1] = "02";
        numberMeters[2] = "03";

      } else if (dato == "2") {
        numberMeters[0] = "01";
        numberMeters[1] = "02";
      } else {
        numberMeters[0] = "01";
      }
    }
    delay(100);


    if (serialUnoString == "") {
      M5.Lcd.println("Inicio obtencion seriales");
      Serial.println("Inicio de obtencion de seriales");
      M5.Lcd.println("#Se require ");
      M5.Lcd.println("serial medidores");
      Serial.println("#Se require serial de medidores");
      delay(3000); // 3000
      M5.lcd.clear();
      String respuesta = sendSerialNumber();
      M5.Lcd.begin();
      delay(1000);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.setTextColor(WHITE);

      if (respuesta == "null"){
        M5.Lcd.print("Serial: ");
        Serial.print("Serial: ");
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.println(respuesta);
        Serial.println(respuesta);
        Serial.println("Serial de medidores no encontrado");
        M5.Lcd.println("Serial de medidores no encontrado");
        Serial.println("Revice conexion con Macrom");
        M5.Lcd.println("Revice conexion con Macrom");
        Serial.println("Favor reiniciar proceso");
        M5.Lcd.println("Favor reiniciar proceso");
        delay(5000);
        respuesta == "";
        M5.lcd.clear();
        M5.Lcd.begin();

        config();
        return;
        
      } else {
        M5.Lcd.println("Serial de medidores: ");
        Serial.println("Serial de medidores: ");
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.println(respuesta);
        Serial.println(respuesta);
      }
    } else {
      medidorUnoModbus = serialUnoString;
      medidorDosModbus = serialDosString;
      medidorTresModbus = serialTresString;
    }
  } else {
    String keyId = readStringFromEEPROM(0);
    projectId = keyId.toInt();
    String dato = readStringFromEEPROM(20);
    if (dato == "3") {
      numberMeters[0] = "01";
      numberMeters[1] = "02";
      numberMeters[2] = "03";
    } else if (dato == "2") {
      numberMeters[0] = "01";
      numberMeters[1] = "02";
    } else {
      numberMeters[0] = "01";
    }
    medidorUnoModbus = serialUnoString;
    medidorDosModbus = serialDosString;
    medidorTresModbus = serialTresString;
  }
  status = 0;
}

const unsigned long TIMEOUT_WIFI = 10000; // 10 segundos

bool conexion() {
  // const char* ssid = "MCA-ITACA"; //Pruebas
  // const char* password = "Itaca#900418160";

  //uso de preferences.h
  prefs.begin("wifi", true); // true = modo lectura
  String ssidString = prefs.getString("ssid", "");
  String passwordString = prefs.getString("pass", "");
  prefs.end();

  const char* ssid = ssidString.c_str();
  const char* password = passwordString.c_str();

  Serial.println("Manejo datos: ");
  Serial.print("User: ");
  Serial.println(ssid);
  Serial.println("Pass: ******");
  // Serial.println(password); // SOLO PRUEBAS MUESTRA CONTRASEÑA WIFI


  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  unsigned long startAttemptTime = millis();

  // Esperar hasta conectar o hasta que se acabe el tiempo
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < TIMEOUT_WIFI) {
    Serial.print(".");
    delay(500); // Espera medio segundo antes de volver a verificar
  }

  if (WiFi.status() == WL_CONNECTED) {
      
    Serial.print("IP asignada: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nNo se pudo conectar a WiFi (timeout)");
    return false;
  }
}

void borrarIPyReiniciar() {
  Preferences prefs;
  prefs.begin("wifi", false);  // acceso escritura
  prefs.remove("ip");          // borra la IP guardada
  prefs.end();

  Serial.println("IP eliminada de memoria.");
  M5.Lcd.println("IP eliminada");
  delay(10000);
 
  Serial.println("Reiniciando dispositivo...");
  M5.Lcd.println("Reiniciando...");
  delay(2000);

  ESP.restart(); // reinicio del ESP32 / M5Stack
}