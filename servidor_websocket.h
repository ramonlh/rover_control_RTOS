
#include <ArduinoJson.h>

WebSocketsServer webSocket = WebSocketsServer(PORT_WEBSOCKET); // #define port_websockets 92

// Tamaño del buffer para almacenar el JSON
#define JSON_BUFFER_SIZE 128
char jsonBuffer[JSON_BUFFER_SIZE]; // Buffer estático para JSON

// Manejar mensajes de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    // Usar un buffer de tamaño fijo para manejar el mensaje
    char message[length + 1]; // Crear un buffer temporal
    memcpy(message, payload, length);
    message[length] = '\0'; // Asegurarse de que la cadena termine en '\0'

    // Intentar parsear el mensaje como JSON
    StaticJsonDocument<200> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, message);
    if (!error) {
      // Extraer valores del JSON para las direcciones (d1, d2, d3, d4) y velocidades (s1, s2, s3, s4)
      int d_trd = jsonDoc["d1"]; // Trasera Derecha (Rueda 1)
      int d_tri = jsonDoc["d2"]; // Trasera Izquierda (Rueda 2)
      int d_ded = jsonDoc["d3"]; // Delantera Derecha (Rueda 3)
      int d_dei = jsonDoc["d4"]; // Delantera Izquierda (Rueda 4)
      int s_trd = jsonDoc["s1"]; // Velocidad de la Trasera Derecha
      int s_tri = jsonDoc["s2"]; // Velocidad de la Trasera Izquierda
      int s_ded = jsonDoc["s3"]; // Velocidad de la Delantera Derecha
      int s_dei = jsonDoc["s4"]; // Velocidad de la Delantera Izquierda
      // Llamar a la función rover_move con los valores extraídos
      rover_move(d_trd, d_tri, d_ded, d_dei, s_trd, s_tri, s_ded, s_dei);
      } 
    else 
      {
      //Serial.println(F("Error parse JSON."));
      // Comprobar el mensaje recibido y realizar las acciones correspondientes
      //Serial.println(message);
      if      (strcmp(message, "f") == 0) { tipo_mov = 1; }
      else if (strcmp(message, "r") == 0) { tipo_mov = 2; }
      else if (strcmp(message, "fl") == 0) { tipo_mov = 3; }
      else if (strcmp(message, "fr") == 0) { tipo_mov = 4; }
      else if (strcmp(message, "rl") == 0) { tipo_mov = 5; }
      else if (strcmp(message, "rr") == 0) { tipo_mov = 6; }
      else if (strcmp(message, "rotl") == 0) { tipo_mov = 7; }
      else if (strcmp(message, "rotr") == 0) { tipo_mov = 8; }
      else if (strcmp(message, "latl") == 0) { tipo_mov = 9; }
      else if (strcmp(message, "latr") == 0) { tipo_mov = 10; }
      else if (strcmp(message, "stop") == 0) { tipo_mov = 0; }
      else if (strcmp(message, "caml") == 0) { angulo_servo_1 += 1; }
      else if (strcmp(message, "camr") == 0) { angulo_servo_1 -= 1; }
      else if (strcmp(message, "camup") == 0) { angulo_servo_2 -= 1; }
      else if (strcmp(message, "camdn") == 0) { angulo_servo_2 += 1; }

      // Manejar el comando de velocidad: "speed:valor"
      else if (strncmp(message, "speed:", 6) == 0) {
        int new_speed;
        if (sscanf(message + 6, "%d", &new_speed) == 1) {
          rover_speed = new_speed;
          }
        }

      // Manejar el comando de luces: "toggle_light"
      else if (strcmp(message, "toggle_light") == 0) {
        //luces_encendidas = !luces_encendidas; // Alternar estado de las luces
        digitalWrite(pin_led_7colores, !digitalRead(pin_led_7colores)); // Cambia el estado del pin
        } 
      }

    }
}

void sendSensorData() {
  if (webSocket.connectedClients() > 0) { // Solo enviar si hay clientes conectados
    // Crear el JSON usando un buffer estático
    snprintf(jsonBuffer, JSON_BUFFER_SIZE, "{\"t\":%.2f, \"h\":%.2f, \"a\":%.2f, \"e\":%.2f, \"g\":%.2f, \"dUS1\":%ld}",
             valores_DHT11.temperature, valores_DHT11.humidity, angleZ, angleY, angleX, dUS1);
    // Enviar los datos a todos los clientes conectados
    webSocket.broadcastTXT(jsonBuffer);
    #ifdef DEBUG
      Serial.print("sendSensorData:"); Serial.println(jsonBuffer);
    #endif
  }
}

void task_websockets(void *pvParameters) {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  #ifdef DEBUG
    Serial.println(F("WS Ok"));
  #endif
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    sendSensorData();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_websockets));
    }
}

