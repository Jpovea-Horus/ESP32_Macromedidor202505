// Verifica si un serial de medidor ya está registrado en la base de datos
String queryDBSerial(const String& serial) {
  // Validación de entrada
  if (serial.isEmpty() || serial == "null") {
    Serial.println("[ERROR] Serial inválido o vacío");
    M5.Lcd.println("Serial inválido");
    delay(1000);
    return "null";
  }

  Serial.printf("[INFO] Consultando DB por serial: %s\n", serial.c_str());
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Verificando serial en DB...");

  // Hacer petición HTTP a la API
  String responDB = httpGETReq(server, serverLan, statusSerialPath, serial);
  Serial.println("[DEBUG] Respuesta de la API:");
  Serial.println(responDB);

  // Parsear JSON
  StaticJsonDocument<300> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, responDB);
  if (error) {
    Serial.print("[ERROR] JSON mal formado: ");
    Serial.println(error.f_str());
    M5.Lcd.println("Error al leer DB");
    return "null";
  }

  // Obtener el valor esperado
  int apiResp = jsonDoc["body"] | 0;  // Si no existe, devuelve 0
  Serial.printf("[INFO] Resultado DB: body = %d\n", apiResp);
  M5.Lcd.printf("Resultado: %d\n", apiResp);
  delay(1500);

  // Devolver resultado como string
  return (apiResp != 0) ? "true" : "false";
}
