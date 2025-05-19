#include "WString.h"
#ifndef defines_h
#define defines_h

#define SIZE 250

#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 26

byte mac[6];
String macAddr = "";
String numberMeters[4];
String minutos = "";
String ssidString = "";
String passwordString = "";
String Dirstring = "";
String meterSerial = "";
String serialUnoString = "";
String serialDosString = "";
String serialTresString = "";
String Time = "";
String macbystring = "";
String registeredMedidor = "false";
String modo = "";
String dir = "";
String x = "0";
String medidorUnoModbus = "";
String medidorDosModbus = "";
String medidorTresModbus = "";
String serialMed = ""; // no usado
String respMed = ""; // no usado

int conteo = 0;
int projectId = 0;
int status = 1;
int idCommunicator = 0;
int nElements = 0;
int LAN = 0;
int axisx = 110;
int axisy = 140;

int cantidadMedidores = 0;

const char* ssid = "";      //Enter SSID
const char* password = "";  //Enter Password
const char* str = "DE:AD:BE:EF:FE:ED";
const char* apiKey = "TOKEN";
const char* host = "esp32";

byte mac1[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 10, 10, 1, 3 };

//routes
String path = "/api/v1/meterhistory/plots/";
String pathCommunicator = "/api/v1/Communicator/";
String pathRegistered = "/api/v1/meter/";
String errorPath = "/api/v1/meterhistory/errorMeter/";
String statusSerialPath = "/api/v1/meter/serial/";

//empresa HORUS SMART CONTROL
// const char* ssid = "MCA-ITACA";            //Enter SSID
// const char* password = "Itaca#900418160";  //Enter Password

// const char* ssid = "Familia Masco";            //Enter SSID
// const char* password = "1143438086";  //Enter Password

// local
// const String server = "http://10.0.0.155:2318";
// const String serverLan = "10.0.0.155";
// const int PORT = 2318;

// server produccion EC2 aws
const String server = "https://www.macrotest.horussmartenergyapp.com";
const String serverLan = "www.macrotest.horussmartenergyapp.com";
const int PORT = 80;

// server develop
// const String server = "https://microservicehsemeasurement.onrender.com";

String plots[16] = { "", "", "", "", "", "" };
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
int httpResponseCode = 0;


#define USE_INTERNAL_PIN_LOOPBACK 2
#define BAUD 9600
#define TEST_UART 2
#define RXPIN 16
#define TXPIN 17
#define DE 32
#define RE 33
String json = "{}";
#endif  //defines_h
