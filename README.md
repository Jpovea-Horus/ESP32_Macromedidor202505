
# ESP32 Macromedidor - Consulta Modbus

Sistema de consulta para medidores que utilizan el protocolo de comunicación **Modbus**, desarrollado para microcontroladores **ESP32** usando el entorno **Arduino**. Este código permite consultar periódicamente medidores a través de interfaces **WiFi** o **Ethernet**, sincronizar la hora vía **NTP**, y realizar la gestión de configuración mediante pantalla y botones integrados.

---

## 📂 Estructura del proyecto

### 1. Archivo de inicio (`index.ino`)
Contiene la inclusión de las librerías principales:

```cpp
#include <M5Stack.h>
#include <esp_task_wdt.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <UniversalTelegramBot.h>
#include <SSLClient.h>
#include <SPI.h>
#include <EthernetLarge.h>
#include <EthernetUdp.h>
#include "CronAlarms.h"
#include "defines.h"
#include "EEPROM.h"
#include "trust_anchors.h"
```

Configura objetos para la conectividad:
- `WiFiClientSecure` para conexiones por WiFi.
- `EthernetClient` para conexiones cableadas.
- Cliente `NTPClient` para sincronización horaria.

---

### 2. `IncreaseWatchdogTime()`
Función que **incrementa el tiempo del watchdog en 10 segundos**, útil para evitar reinicios durante operaciones largas.

---

### 3. `setup()`
- Inicializa la pantalla (LCD del M5Stack).
- Configura velocidad del puerto serie y pines RX/TX.
- Verifica si hay datos guardados en la **EEPROM** (como número de serie del medidor y red).
- Si no hay datos, llama a la función `config()` para configuración inicial.

---

### 4. `config()`
Gestiona la configuración del sistema:

- Detecta si el modo es **WiFi** (`1`) o **LAN** (`0`).
- Solicita al usuario:
  - Número de serie del medidor.
  - Dirección Modbus.
  - En modo WiFi: nombre y contraseña de la red.
  - En modo LAN: realiza una conexión automática (requiere cable Ethernet conectado).
- Sincroniza la hora del sistema vía NTP.
- Programa:
  - **Reinicio automático diario.**
  - **Consulta periódica cada 10 minutos** al medidor.

---

### 5. `loop()`
- Detecta si se presiona el botón C (reset de EEPROM y reinicio del sistema).
- Llama a `Cron.delay()` para gestionar tareas temporizadas.
- Ejecuta la función `ConsultaMedidorModbus()` cada 10 minutos.

---

## 🔁 Funcionalidades principales

- Consulta periódica de medidores Modbus.
- Detección y configuración automática de red (WiFi o Ethernet).
- Almacenamiento de parámetros en EEPROM.
- Sincronización horaria por NTP.
- Visualización de estado y datos en pantalla.
- Reinicio programado y botón de restauración.

---

## 📌 Requisitos

- **M5Stack Core** u otro ESP32 compatible.
- Arduino IDE con bibliotecas:
  - M5Stack
  - ArduinoJson
  - CronAlarms
  - EthernetLarge
  - UniversalTelegramBot
  - NTPClient
  - SSLClient
- Conexión a internet (WiFi o cable Ethernet).

---

## 🧠 Uso

1. Conecta el dispositivo por USB.
2. Carga el código usando Arduino IDE.
3. En el primer arranque:
   - Introduce número de serie, dirección del medidor y parámetros de red.
4. El sistema realizará automáticamente la sincronización horaria y comenzará las consultas Modbus.
5. Si deseas reiniciar la configuración, presiona el botón A durante la operación.

---

## 📄 Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE` para más información.

---

## 🙌 Agradecimientos

Version 1 desarrollada por Anderson camero. 
Version 2 desarrollada y actualizada por Jefry Povea.
