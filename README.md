# Sistema de consulta para Macromedidor portocolo Modbus / prod
codigo creado en arduino para realizar las consultas de  medidores que usen el protocolo de comunicacion Modbus, este codigo posee lo siguiente 

## Indice
-[archivo de inicio](#archivo-de-inicio)
    -[Index](#index)
    -[IncreaseWatchdogTime](#increasewatchdogtime)
    -[Setup](#setup)
    -[Config](#config)
    -[Loop](#loop)









## Archivo de inicio

### Index
contiene las diferentes librerias a utilizar como lo son:
 <M5Stack.h>,<esp_task_wdt.h>, <ArduinoJson.h>,<HardwareSerial.h>
<Arduino_JSON.h>, <WiFi.h>, <WiFiUdp.h>, <WiFiClientSecure.h>
<HTTPClient.h>, <stdlib.h>, <NTPClient.h>, <UniversalTelegramBot.h>
<SSLClient.h>, <SPI.h>, <EthernetLarge.h>, <EthernetUdp.h>, "CronAlarms.h"
"defines.h", "EEPROM.h", "trust_anchors.h"
se configura el objeto WiFiClientSecure para la conectividad por medio del Wifi, y EthernetClient si la 
conectivida es por medio de cable de red
se inicializa un cliente NTPClient para sincronizar la fecha y hora del equipo

### IncreaseWatchdogTime
incrementa el tiempo del watchdog en 10 segundos.

### Setup
inicializa la pantalla, luego establece la velocidad, paridad y pines de RX y TX, verifica si la  
eeprom tiene datos guardados 
como el serial del medidor, la direccion, tipo de red si es WIFI o LAN
si no encuentra datos guardados se redireccioa a la funcion config

### Config
verifica que modo esta activo 1 para WiFi y 0 para LAN, solicitara que se ingrese el serial del medidor 
y seguido a eso como no encuentra direccion alguna solicitara la direccion del equipo
de pendidendo del modo escogido inicial mente si es WIFI, solocitara el nombre de la red y su contraseña 
pero si el LAN este ejecutara una secuensia por defecto y se conectara a la red 

Nota: debe estar concetado el cable Ethernet 

una ves finalizada las configuraciones antes mencionadas, sincroniza el relog interno, configura una 
funcion que reinicia el proceso a una determinada hora del dia y configura una llamada cada 10 minutos 
en todo el transcurso del dia que son las veces que se realizan las consultas al medidor

### Loop 
tiene una funcion que se utiliza al presionar el pulsador A esto reinicia la eeprom esto con el fin de 
realizar un recet de dispositivo, y en la funcion princival esta llamando a Cron.delay que es la que 
lleva el conteo de los segundos del dia, y llama cada 10 minutos a la funcion ConsultaMedidorModbus 

