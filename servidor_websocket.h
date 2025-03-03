
WebSocketsServer webSocket = WebSocketsServer(PORT_WEBSOCKET); // #define port_websockets 92

void sendSensorData() {
  if (webSocket.connectedClients() > 0) { // Solo enviar si hay clientes conectados
    String json = "{\"temp\":" + String(valores_DHT11.temperature) +     // estos son valores de prueba
                  ", \"hum\":" + String(valores_DHT11.humidity) + 
                  ", \"azim\":" + String(angleZ) + 
                  ", \"elev\":" + String(angleY) + 
                  ", \"giro\":" + String(angleX) + 
                  ", \"distUS1\":" + String(distanciaUS1) + "}";
//                  ", \"distUS1\":" + String(distanciaUS1,0) + "}";
    //Serial.println(json);
    webSocket.broadcastTXT(json); // Envía los datos a todos los clientes
    }
}

// Manejar mensajes de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = (char*)payload;
        //Serial.println(message);
        if (message == "adelante") {
          rover_adelante();          }
        else if (message == "atras") {
          rover_atras();          }
        else if (message == "giroizda") {
          rover_giro_izda();          }
        else if (message == "girodcha") {
          rover_giro_dcha();          }
        else if (message == "atrasizda") {
          rover_atras_izda();          }
        else if (message == "atrasdcha") {
          rover_atras_dcha();          }
        else if (message == "rotizda") {
          rover_rot_izda();          }
        else if (message == "rotdcha") {
          rover_rot_dcha();          }
        else if (message == "latizda") {
          rover_lat_izda();          }
        else if (message == "latdcha") {
          rover_lat_dcha();          }
        else if (message == "stop") {
          rover_stop();          }
        else if (message == "camizda") {
          angulo_servo_1 += 1;          }
        else if (message == "camdcha") {
          angulo_servo_1 -= 1;          }
        else if (message == "camarriba") {
          angulo_servo_2 -= 1;           }
        else if (message == "camabajo") {
          angulo_servo_2 += 1;          }
        else if (message == "camstop") {          }
    }
}

void init_websockets()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Servidor WebSocket iniciado");
}

