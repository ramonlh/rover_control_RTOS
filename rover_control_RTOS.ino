

#define DEBUG

// librerías genéricas
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>

// includes del proyecto
#include "defines.h"
#include <wifi_connect.h>  
#include "ota.h"

#include <sistema_ficheros.h>  
#include "giroscopio.h"
#include "4motores.h"
#include "fecha_hora.h"
#include "sensor_ultrasonidos.h"
#include "dht11.h"
#include "servomotores.h"
#include "radar_vl53l0x.h"
#include "servidor_web.h"
#include "servidor_websocket.h"

#include "servidor_ftp.h"
#include "radio_control.h"
#include "radarhumano.h"

#define OFF LOW
#define ON HIGH

void setup() {
  Serial.begin(115200);
  // init conexiones y servidores

  ini_sistema_ficheros();

  if (modo_conex == 0)
    {
    Serial.println("Modo: 0 STA+AP+RC");
    init_wifi();
    init_webserver();
    init_ftp_server();  
    xTaskCreate(task_websockets, "Task WS", tamano_task_websockets, NULL, prioridad_task_websockets, NULL); // comprobado 2048
    }
  else if (modo_conex == 1)
    {
    Serial.println("Modo: 1 AP+RC");
    init_wifi();
    init_webserver();
    init_ftp_server();    
    xTaskCreate(task_websockets, "Task WS", tamano_task_websockets, NULL, prioridad_task_websockets, NULL); // comprobado 2048
    }
  else
    {
    Serial.println("Modo: 2 RC");
    }

  pinMode(pin_led_7colores, OUTPUT);
  digitalWrite(pin_led_7colores, HIGH);
  delay(2000);
  digitalWrite(pin_led_7colores, LOW);

  xTaskCreate(task_dht11, "Task DHT11", tamano_task_DHT, NULL, prioridad_task_DHT, NULL); // comprobado 1024
  xTaskCreate(task_radarhumano, "Task Radar Humano", tamano_task_radarhumano, NULL, prioridad_task_radar, NULL); // comprobado 1024
  xTaskCreate(task_radiocontrol, "Task RC", tamano_task_radiocontrol, NULL, prioridad_task_radiocontrol, NULL); // comprobado 2048
  xTaskCreate(task_giroscopio, "Task Giros", tamano_task_giroscopio, NULL, prioridad_task_giroscopio, NULL); // comprobado 2048
  xTaskCreate(task_servomotores, "Task Servos", tamano_task_servomotores, NULL, prioridad_task_servomotores, NULL); // comprobado 2048
  xTaskCreate(task_motores, "Task motores", tamano_task_motores, NULL, prioridad_task_motores, NULL); // 
  enabled_ultrasound = 0;
  xTaskCreate(task_ultrasonidos, "Task UltraS", tamano_task_ultrasonidos, NULL, prioridad_task_ultrasonidos, NULL); // comprobado 1024
  xTaskCreate(task_radar, "Task Radar", tamano_task_radar, NULL, prioridad_task_radar, NULL); // comprobado 1024

}

void loop() {
  //Serial.println("===");
  //Serial.print("modo_conex:"); Serial.println(modo_conex);
  // Manejo de conexiones y servidores  
  if (modo_conex == 0)
    {
    webSocket.loop();
    webserver.handleClient();
    ArduinoOTA.handle();
    ftpSrv.handleFTP();
    }

  else if (modo_conex == 1)
    {
    webSocket.loop();
    webserver.handleClient();
    ArduinoOTA.handle();
    ftpSrv.handleFTP();
    }
  else if (modo_conex == 2)
    {

    }

  // Manejo de sensores y actuadores
  //handle_obstaculoUS();   // mira la distancia y controla si hay que parar el coche
  //if ((rumbo_adelante==1) || (rumbo_adelante==2))
    //{
    //conservar_rumbo();
    //}

}