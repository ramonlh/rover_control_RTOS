
WebSocketsServer webSocket = WebSocketsServer(PORT_WEBSOCKET); // #define port_websockets 92

// Tamaño del buffer para almacenar el JSON
#define JSON_BUFFER_SIZE 128
char jsonBuffer[JSON_BUFFER_SIZE]; // Buffer estático para JSON

void sendSensorData() {
  if (webSocket.connectedClients() > 0) { // Solo enviar si hay clientes conectados
    // Crear el JSON usando un buffer estático
    snprintf(jsonBuffer, JSON_BUFFER_SIZE, "{\"t\":%.2f, \"h\":%.2f, \"a\":%.2f, \"e\":%.2f, \"g\":%.2f, \"dUS1\":%ld}",
             valores_DHT11.temperature, valores_DHT11.humidity, angleZ, angleY, angleX, dUS1);
    // Enviar los datos a todos los clientes conectados
    webSocket.broadcastTXT(jsonBuffer);
  }
}

// Manejar mensajes de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    // Usar un buffer de tamaño fijo para manejar el mensaje
    char message[length + 1]; // Crear un buffer temporal
    memcpy(message, payload, length);
    message[length] = '\0'; // Asegurarse de que la cadena termine en '\0'
    // Comprobar el mensaje recibido y realizar las acciones correspondientes
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
    }
}
void init_websockets()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println(F("WS Ok"));
}

