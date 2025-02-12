
WebSocketsServer webSocket = WebSocketsServer(PORT_WEBSOCKET); // #define port_websockets 92

void sendSensorData() {
  if (webSocket.connectedClients() > 0) { // Solo enviar si hay clientes conectados
    String json = "{\"temp\":" + String(23.2) + 
                  ", \"hum\":" + String(44.5) + 
                  ", \"azim\":" + String(61.1) + 
                  ", \"elev\":" + String(15.5) + 
                  ", \"giro\":" + String(13.5) + "}";
//    Serial.println(json);
    webSocket.broadcastTXT(json); // Envía los datos a todos los clientes
    }
}

// Manejar mensajes de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = (char*)payload;
        if (message == "adelante") {
          rover_adelante();          }
        else if (message == "atras") {
          rover_atras();          }
        else if (message == "giroizda") {
          rover_giro_izda();          }
        else if (message == "girodcha") {
          rover_giro_dcha();          }
        else if (message == "rotizda") {
          rover_rot_izda();          }
        else if (message == "rotdcha") {
          rover_rot_dcha();          }
        else if (message == "stop") {
          rover_stop();          }
        else if (message == "camizda") {
          angulo_servo_2 += 1;          }
        else if (message == "camdcha") {
          angulo_servo_2 -= 1;          }
        else if (message == "camup") {
          angulo_servo_1 -= 1;          }
        else if (message == "camdown") {
          angulo_servo_1 += 1;          }
        else if (message == "camstop") {          }
    }
}

void init_websockets()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Servidor WebSocket iniciado");
}

