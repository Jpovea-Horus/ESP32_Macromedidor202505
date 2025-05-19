// #include <stdio.h>
#ifndef packagesent_h
#define packagesent_h

String httpPOSTReq(String serverNamePost, String serverLan, String Path, String dato, String Serial) {
  M5.begin();
  M5.Power.begin();
  M5.lcd.clear();
  if (LAN == 1) {
    String serverPath = serverLan;
    String url = Path + Serial;
    if (client1.connect(serverPath.c_str(), PORT)) {
      client1.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + serverPath + " \r\n" + "Authorization: Bearer " + apiKey + "\r\n" + "Content-Type: application/json\r\n" + "Content-Length: " + String(dato.length()) + "\r\n\r\n");
      client1.print(dato);
      delay(1000);
      String line;
      while (client1.available() == 0)
        ;
      while (client1.available()) {
        line = client1.readStringUntil('\n');
      }
      int pos = line.indexOf("body");
      client1.println("Connection: close");
      client1.println();
      // Serial.print("Se enviaron datos de ");
      return line;
    } else {
      // Serial.print("Error enviando datos de ");
      return "ERROR";
    }
  } else {
    String postData = dato;
    HTTPClient http;
    String serverPath = serverNamePost + Path + Serial;
    http.begin(serverPath.c_str());
    // Send HTTP POST reques
    http.addHeader("apiKey", "test");
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(dato);
    String Postdata = "{}";
    if (httpResponseCode > 0) {
      if (httpResponseCode >= 200 && httpResponseCode < 300) {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.println(httpResponseCode);
        M5.lcd.setTextSize(2);
        Postdata = http.getString();
        M5.Lcd.println("successfully saved");
        // Serial.print("successfully saved");
        delay(10000);
        M5.Lcd.setTextColor(WHITE);
        M5.lcd.clear();
      } else if (httpResponseCode >= 400 && httpResponseCode < 500) {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println(httpResponseCode);
        M5.lcd.setTextSize(2);
        Postdata = http.getString();
        M5.Lcd.println("Bad Request");
        // Serial.print("successfully saved");
        delay(10000);
        M5.Lcd.setTextColor(WHITE);
        M5.lcd.clear();
      }
    } else {
      String code = JSON.stringify(httpResponseCode);
      if (code == "-1") {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println("Error de conexion");
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.println("Servidor no responde");
        M5.Lcd.println("intenta nuevamente");
        delay(9000);
        M5.Lcd.setTextColor(WHITE);
      }
    }
    // Free resources
    http.end();
    esp_task_wdt_reset();
    M5.lcd.clear();
    return Postdata;
  }
}

// Realiza una solicitud HTTP GET al servidor usando WiFi o LAN, y retorna la respuesta como string
String httpGETReq(String serverNamePost, String serverLan, String Path, String Serial) {

  // ----- MODO WiFi -----
  if (LAN == 0) {
    HTTPClient http;

    // Construimos la URL de destino
    String serverPath = serverNamePost + Path + Serial;

    // Iniciamos la conexion HTTP
    http.begin(serverPath.c_str());

    // Agregamos cabeceras necesarias
    http.addHeader("apiKey", "test");
    http.addHeader("Content-Type", "application/json");

    // Realizamos la solicitud GET
    int httpResponseCode = http.GET();
    String Postdata = "{}";  // Valor por defecto si no hay respuesta útil

    // Si hubo respuesta válida
    if (httpResponseCode > 0) {
      M5.Lcd.setCursor(0, 0);
      M5.lcd.setTextSize(2);

      if (httpResponseCode >= 200 && httpResponseCode < 300) {
        // Éxito: código 2xx
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.println(httpResponseCode);
        Postdata = http.getString(); // Obtener contenido del cuerpo de respuesta
        M5.Lcd.println("successfully saved");
      } else if (httpResponseCode >= 400 && httpResponseCode < 500) {
        // Error del cliente: código 4xx
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println(httpResponseCode);
        Postdata = http.getString();
        M5.Lcd.println("Bad Request");
      }

      delay(10000); // Pausa para leer mensajes
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.clear();
    } 
    else {
      // No se pudo conectar con el servidor
      String code = JSON.stringify(httpResponseCode);  // Convertimos código a texto
      if (code == "-1") {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println("Error de conexion");
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.println("Servidor no responde");
        M5.Lcd.println("intenta nuevamente");
        delay(9000);
        M5.Lcd.setTextColor(WHITE);
      }
    }

    // Cerramos la conexion y liberamos recursos
    http.end();
    esp_task_wdt_reset();  // Reiniciamos el watchdog
    M5.Lcd.clear();
    return Postdata;
  }

  // ----- MODO LAN (Ethernet) -----
  else {
    String serverPath = serverLan;
    String url = Path + Serial;

    // Intentamos conectar al servidor por puerto definido
    if (client1.connect(serverPath.c_str(), PORT)) {

      // Enviamos manualmente una solicitud HTTP GET
      client1.print(
        String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + serverPath + " \r\n" +
        "Authorization: Bearer " + apiKey + "\r\n" +
        "Content-Type: application/json\r\n\r\n"
      );

      delay(1000); // Esperamos que llegue la respuesta

      // Leemos toda la respuesta línea por línea
      String line;
      while (client1.available() == 0);  // Espera hasta que haya datos
      while (client1.available()) {
        line = client1.readStringUntil('\n');
      }

      // Cerramos conexion
      client1.println("Connection: close");
      client1.println();

      // Devolvemos la última línea de respuesta
      return line;
    } 
    else {
      // Falló la conexion
      return "ERROR";
    }
  }
}


#endif