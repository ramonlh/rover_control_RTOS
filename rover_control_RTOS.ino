
// librerías genéricas
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>

// includes del proyecto
#include "defines.h"
#include <wifi_manager.h>  
#include "ota.h"

#include <sistema_ficheros.h>  
#include "giroscopio.h"
#include "4motores.h"
#include "fecha_hora.h"
#include "sensor_ultrasonidos.h"
#include "dht11.h"
#include "servomotores.h"
#include "servidor_web.h"
#include "servidor_websocket.h"

#include "servidor_ftp.h"
#include "radio_control.h"
#include "radar.h"
#include "manage_ips.h"

#define OFF LOW
#define ON HIGH

constexpr uint32_t timedelay1 = 1000;
constexpr uint32_t timedelay10 = 10000;
constexpr uint32_t timedelay60 = 60000;
uint32_t time1 = 0;
uint32_t time10 = 0;
uint32_t time60 = 0;

void cosas_cada_segundo()
{
  unixtime++;
  //sendSensorData();  
  time1 = millis();
}

void cosas_cada_10segundos()
{
  //sendSensorData();  
  //send_IPs();
  time10 = millis();
}

void cosas_cada_60segundos()
{
  time60 = millis();
}

  // Tareas periódicas
void handle_tareas_periodicas()
{
  uint32_t millisact = millis();
  if ((millisact - time1) > timedelay1) { cosas_cada_segundo(); }
  if ((millisact- time10) > timedelay10) { cosas_cada_10segundos(); }
  if ((millisact - time60) > timedelay60) { cosas_cada_60segundos(); }
}

  // Manejo de comandos por serial
void handle_serial_commands()
{
  if (Serial.available())
    {
     char car = Serial.read();
     Serial.printf("Leído: %c\n", car);
    }
}

void setup() {
  Serial.begin(115200);
  // init conexiones y servidores

  ini_sistema_ficheros();
  init_wifi_manager();

  init_webserver();
  init_websockets();
  init_ota();
  init_ftp_server();

  pinMode(pin_led_7colores, OUTPUT);
  digitalWrite(pin_led_7colores, HIGH);
  delay(2000);
  digitalWrite(pin_led_7colores, LOW);

  init_manageips();

  xTaskCreate(task_giroscopio, "Task Giros", 2048, NULL, 2, NULL); // comprobado 2048
  xTaskCreate(task_dht11, "Task DHT11", 2048, NULL, 1, NULL); // comprobado 1024
  xTaskCreate(task_ultrasonidos, "Task Ultras", 3000, NULL, 2, NULL); // comprobado 1024
  xTaskCreate(task_radar, "Task Radar", 1024, NULL, 1, NULL); // comprobado 1024
  xTaskCreate(task_servomotores, "Task Servos", 2048, NULL, 3, NULL); // comprobado 2048
  xTaskCreate(task_motores, "Task Motores", 2048, NULL, 3, NULL); // 
  xTaskCreate(task_radiocontrol, "Task RC", 4000, NULL, 2, NULL); // comprobado 2048
  
  // Configura el servidor DNS después de la conexión. Necesario si se usa la librería WiFi_Manager
  IPAddress dns(8, 8, 8, 8);  // DNS de Google (puedes usar otro)
  IPAddress subnet(255, 255, 255, 0);  // DNS de Google (puedes usar otro)
  WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), subnet, dns);  // Establece subnet y DNS  
}

void loop() {

  // Manejo de conexiones y servidores  
  wifimanager_loop();
  webSocket.loop();
  webserver.handleClient();
  ArduinoOTA.handle();
  ftpSrv.handleFTP();
  
  // Manejo de sensores y actuadores
  handle_obstaculoUS();

  if ((rumbo_adelante==1) || (rumbo_adelante==2))
    {
    conservar_rumbo();
    }

  // Tareas periódicas
  handle_tareas_periodicas();

  // Manejo de comandos por serial
  handle_serial_commands();

   manage_ips_loop();
}