
// librerías genéricas
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <HTTPClient.h>
//#include "Arduino_JSON.h"
#include <Wire.h>
#include <UnixTime.h>
//#include <JY901.h>
//#include DHTesp.h" 

// includes del proyecto
#include "defines.h"
#include <wifi_manager.h>  
//#include "conectar_wifi.h"
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
#include "servidor_tcpsocket.h"
#include "servidor_ftp.h"
#include "radio_control.h"
#include "radar.h"
//#include "pantalla.h"

#define OFF LOW
#define ON HIGH

long timedelay1 = 1000;
long timedelay10 = 10000;
long timedelay60 = 60000;
long time1 = 0;
long time10 = 0;
long time60 = 0;

void cosas_cada_segundo()
{
  unixtime++;

  time1 = millis();
}

void cosas_cada_10segundos()
{
  sendSensorData();  
  time10 = millis();
}

void cosas_cada_60segundos()
{
  time60 = millis();
}

// está aquí para ejemplo
void task_ejemplo(void *pvParameters) {  // This is a task.
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1) {       
    //Serial.println("Task ejemplo ejecutada");                   // A Task shall never return or exit.;
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(60000));
  }
}

  // Tareas periódicas
void handle_tareas_periodicas()
{
  if ((millis() - time1) > timedelay1) { cosas_cada_segundo(); }
  if ((millis() - time10) > timedelay10) { cosas_cada_10segundos(); }
  if ((millis() - time60) > timedelay60) { cosas_cada_60segundos(); }
}

  // Manejo de comandos por serial
void handle_serial_commands()
{
  if (Serial.available())
    {
     char car = Serial.read();
     Serial.print("leido:"); Serial.print(car); Serial.println(":");
     if (car == 'i')
       {
       angulo_servo_1=10; 
       angulo_servo_2=10; 
       }
     if (car == 'm')
       {
       angulo_servo_1=90; 
       angulo_servo_2=90; 
       }
     if (car == 'f')
       {
       angulo_servo_1=150; 
       angulo_servo_2=150; 
       }
     Serial.println(angulo_servo_1);
     Serial.println(angulo_servo_2);
    }
}

void setup() {
  Serial.begin(115200);
  // init conexiones y servidores
  ini_sistema_ficheros();
  init_wifi_manager();
  init_webserver();
  init_websockets();
  //init_tcpserver();
  init_ota();
  init_ftp_server();

  // init motores
  init_motores();

  //init_pantalla();

  pinMode(pin_led_7colores, OUTPUT);
  digitalWrite(pin_led_7colores, HIGH);
  delay(2000);
  digitalWrite(pin_led_7colores, LOW);

  xTaskCreate(task_ejemplo, "Task Ejemplo", 2048, NULL, 1, NULL);
  xTaskCreate(task_radiocontrol, "Task Radio Control", 2048, NULL, 2, NULL);
  xTaskCreate(task_giroscopio, "Task Giroscopio", 2048, NULL, 2, NULL);
  xTaskCreate(task_dht11, "Task DHT11", 2048, NULL, 1, NULL);
  xTaskCreate(task_ultrasonidos, "Task Ultrasonidos", 2048, NULL, 2, NULL);
  xTaskCreate(task_radar, "Task Radar", 2048, NULL, 1, NULL);
  xTaskCreate(task_servomotores, "Task Servomotores", 4096, NULL, 3, NULL);

}

void loop() {
  // Manejo de conexiones y servidores  
  wifimanager_loop();
  //handle_tcpserver();
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

}