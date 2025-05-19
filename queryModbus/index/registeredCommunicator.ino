#include "lib/defines.h"
#include "network/packagesent.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

void sendRegiterdCommunicator(String serial) {
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.println("enviando registro de controlador...");
  delay(2000);
  // en produccion es TypeCommunicatorId, ProjectId
  String body;
  StaticJsonDocument<300> doc;
  doc["TypeCommunicatorId"] = 1;
  doc["ProjectId"] = projectId;
  doc["name"] = "test2";
  doc["idCommunicator"] = String("K001V6") + macAddr;
  doc["state"] = "1";
  doc["model"] = "M5STACK";
  serializeJson(doc, body);
  delay(1000);
  String rstDB = httpPOSTReq(server, serverLan, pathCommunicator, body, serial);
  Serial.println(rstDB);
  delay(1000);
  StaticJsonDocument<300> resp;
  DeserializationError err = deserializeJson(resp, rstDB);
  if (err) {
    Serial.print("Deserialization failed with code");
    Serial.println(err.f_str());
  } else {
    idCommunicator = resp["body"];
    delay(10000);
    writeStringToEEPROM(123, String(idCommunicator));
  }
}

String sendRegisterdMeter(String serial) {
  M5.Lcd.println("enviando registro del medidor: ");
  M5.Lcd.print(serial);
  delay(500);
  String body;
  StaticJsonDocument<300> doc;

  // en produccion son: CommunicatorId, relationTc
  doc["CommunicatorId"] = idCommunicator;
  doc["serial"] = serial;
  doc["model"] = "SMD630MCTV2";
  doc["relationTc"] = "200/5";
  doc["maximumCurrent"] = "5000";
  doc["isNewMeter"] = true;
  doc["enabled"] = true;
  doc["powerFactor"] = 40;

  JsonObject variables = doc.createNestedObject("variables");
  variables["VFA"] = 120;
  variables["VFB"] = 122;
  variables["VFC"] = 125;

  JsonObject variablesFactor = doc.createNestedObject("variablesFactor");
  variablesFactor["VFA"] = 120;
  variablesFactor["VFB"] = 122;
  variablesFactor["VFC"] = 125;

  serializeJson(doc, body);
  delay(500);
  Serial.print("serial: ");
  Serial.println(serial);
  String rstDB = httpPOSTReq(server, serverLan, pathRegistered, body, serial);
  M5.Lcd.println(rstDB);
  M5.Lcd.println(body);
  return rstDB;
}
