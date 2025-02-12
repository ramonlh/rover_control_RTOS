
#include <UnixTime.h>

WebServer webserver(PORT_WEBSERVER);    // #define port_servidor_web 91

const char head_1[512] PROGMEM =    // cabeceras
  "<!DOCTYPE html>\n"
  "<html lang=\"es\">\n"
    "<head>\n"
      "<meta charset=\"UTF-8\">\n"
      "<title>Control Rover</title>\n"
      "<link rel=\"icon\" href=\"data:,\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
      "<!-- Meta tag para hacer la página responsive -->";

const char head_2[1024] PROGMEM =   // Styles
      "<style>\n"
        "body { font-family: Arial, sans-serif; text-align: center; }\n"
        "button { padding: 10px 20px; font-size: 16px; margin: 10px; }\n"
        "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"
        "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"
        ".button {display: inline-block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;"
        "text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n"
        ".button-on {background-color: #3498db;}\n"
        ".button-on:active {background-color: #2980b9;}\n"
        ".button-off {background-color: #34495e;}\n"
        ".button-off:active {background-color: #2c3e50;}\n"
        "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";

const char head_3[1024] PROGMEM =
        ".indicators { display:flex; justify-content: space-around; margin:20px 0;}"
        ".indicator { text-align: center;}"
        ".vehicle {width: 150px;  height: 150px; transition: transform 0.3s ease; transform-origin: center center;}"
        ".progress-bar {width: 200px; height: 30px; background-color: #e0e0e0; border-radius: 10px; overflow: hidden; position: relative; margin: 10px auto; }"
        ".progress-bar-fill {height: 100%; background-color: #3498db; width: var(--percent, 0%); }"
        ".progress-bar::after {content: attr(data-value); font-size: 14px; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); color: #444444; }"
        /* Media Queries para dispositivos pequeños */
        "@media (max-width: 600px) { button { font-size: 14px; padding: 8px 16px;}"
          ".vehicle { width: 80px; height: 80px;}"
        ".progress-bar { width: 80px;} progress-bar-fill { height: 10px;}"
        "#streamImg {width: 240px;height: 180px;}"
        "h1 {font-size: 24px;}"
        "p {font-size: 12px;}"
        ".inline-buttons {flex-direction: column; gap: 5px;}}"
      "</style>\n"
    "</head>\n"
    "<body>\n"
    "<h3>Control Rover</h3>\n";

const char head_4[512] PROGMEM =   // barras de temperatura y humedad
    "<span id=\"temp\" style=\"display: none;\">--</span>\n"
    "<span id=\"hum\" style=\"display: none;\">--</span>\n"
    "<div class=\"indicators\">\n"
      "<div class=\"indicator\">Temperatura\n"
        "<div id=\"tempBar\" class=\"progress-bar\" data-value=\"0%\">\n"
          "<div class=\"progress-bar-fill\" style=\"--percent: 0%;\"></div>\n"
        "</div>"
      "</div>\n"
      "<div class=\"indicator\">Humedad"
        "<div id=\"humBar\" class=\"progress-bar\" data-value=\"0%\">"
          "<div class=\"progress-bar-fill\" style=\"--percent: 0%;\"></div>"
        "</div>"
      "</div>"
    "</div>\n";

//const char head_5[2] PROGMEM ="";

const char head_6[512] PROGMEM =    // imágenes de posición del coche
    "<div class=\"indicators\">\n"
      "<div class=\"indicator\">\n"
        "<p>Rumbo</p>\n"
        "<img src=\"/cochearriba\" alt=\"Vehiculo\" id=\"vehicleazimut\" class=\"vehicle\">"
        "<p><span id=\"azim\">--</span>°</p>"
      "</div>\n"
      "<div class=\"indicator\">"
        "<p>Cabeceo</p>"
        "<img src=\"/cochelado\" alt=\"Vehiculo\" id=\"vehicleelevacion\" class=\"vehicle\">"
        "<p><span id=\"elev\">--</span>°</p>"
      "</div>\n"
      "<div class=\"indicator\">"
        "<p>Alabeo</p>"
        "<img src=\"/cochetrasera\" alt=\"Vehiculo\" id=\"vehiclegiro\" class=\"vehicle\">"
        "<p><span id=\"giro\">--</span>°</p>"
      "</div>\n"
    "</div>\n";      

const char head_7[1024] PROGMEM =    // botones de movimiento cámara
    "<button id=controlButton9 onmousedown=\"startMovingServo2('camup')\" onmouseup=\"stopMovingServo2()\">&#9650;</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton7 onmousedown=\"startMovingServo1('camizda')\" onmouseup=\"stopMovingServo1()\">&#9664;</button>\n</div>\n"
      "<div>"
        "<img id=\"streamImg\" src=\"http://192.168.11.120:81/stream\" alt=\"cam\" width=\"320\" height=\"240\" >\n" //imagen de la cámara
      "</div>\n"
      "<div>\n"
        "<button id=controlButton8 onmousedown=\"startMovingServo1('camdcha')\" onmouseup=\"stopMovingServo1()\">&#9654;</button>\n"
      "</div>"
    "</div>\n"
    "<button id=controlButton10 onmousedown=\"startMovingServo2('camdown')\" onmouseup=\"stopMovingServo2()\">&#9660;</button><br>\n";

const char head_8[1200] PROGMEM =   // botones movimiento coche
    "<button id=controlButton1 onmousedown=\"sendCommand('giroizda')\" onmouseup=\"sendCommand('stop')\">Giro Izda</button>\n"
    "<button id=controlButton2 onmousedown=\"sendCommand('adelante')\" onmouseup=\"sendCommand('stop')\">Adelante</button>\n"
    "<button id=controlButton3 onmousedown=\"sendCommand('girodcha')\" onmouseup=\"sendCommand('stop')\">Giro Dcha</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton4 onmousedown=\"sendCommand('latizq')\" onmouseup=\"sendCommand('stop')\">Lat Izda</button></div>\n"
      "<div>"
        "<img src=\"/cochearriba\" alt=\"Vehiculo\" id=\"vehicleinutil\" class=\"vehicle\">"  // aquí va la imagen de la cámara
      "</div>\n"
      "<div>\n"
        "<button id=controlButton6 onmousedown=\"sendCommand('latder')\" onmouseup=\"sendCommand('stop')\">Lat Dcha</button>\n"
      "</div>"
    "</div>\n"
    "<button id=controlButton12 onmousedown=\"sendCommand('giroizda')\" onmouseup=\"sendCommand('stop')\">Atras Izda</button>\n"
    "<button id=controlButton13 onmousedown=\"sendCommand('atras')\" onmouseup=\"sendCommand('stop')\"> Atras  </button>\n"
    "<button id=controlButton14 onmousedown=\"sendCommand('girodcha')\" onmouseup=\"sendCommand('stop')\">Atras Der</button><br>\n";

//const char head_9[512] PROGMEM ="";

const char head_10[512] PROGMEM =
  "<button id=controlButton11 onmousedown=\"sendCommand('rotizda')\" onmouseup=\"sendCommand('stop')\">Rot Izda</button>\n"
  "<button id=controlButton14 onmousedown=\"sendCommand('rotdcha')\" onmouseup=\"sendCommand('stop')\">Rot Dcha</button><br>\n"
  // Botones de velocidad en la misma línea -->
  "<div class=\"inline-buttons\">\n"
    "<button onclick=\"changeSpeed(-500)\">---</button>\n"
    "Velocidad:<span id=\"rover_speed_display\">1000</span>\n"
    "<button onclick=\"changeSpeed(+500)\">+++</button>\n"
  "</div>\n";

const char script_01[128] PROGMEM =
  "<script>\n"
    "let ws;\n"
    "let rover_speed=1000;\n" // Velocidad inicial
    // Conectar WebSocket
    "function connectWebSocket() {\n"
      "ws=new WebSocket(`ws://${location.hostname}:";
const char script_02[1024] PROGMEM =    
    "/`);\n"
    "ws.onopen = () => {\n"
      "console.log(\"WebSocket conectado\");};\n"
    "ws.onmessage = (event) => {"
      "console.log(\"Datos recibidos:\", event.data);"
      "try {"
      "let data = JSON.parse(event.data);"
      "document.getElementById(\"azim\").innerText = data.azim.toFixed(1);"
      "document.getElementById(\"elev\").innerText = data.elev.toFixed(1);"
      "document.getElementById(\"giro\").innerText = data.giro.toFixed(1);"
      "document.getElementById(\"temp\").innerText = data.temp.toFixed(1);"
      "document.getElementById(\"hum\").innerText = data.hum.toFixed(1);"
       // Actualizar indicadores gráficos
      "updateIndicators(data.azim, data.elev, data.giro, data.temp, data.hum);"
      "} catch (e) {"
      "console.error(\"Error al procesar datos:\", e);"
      "}"
      "};";
       
const char script_03[1024] PROGMEM =    
    "ws.onclose = () => {\n"
      "console.log(\"WebSocket desconectado\");\n"
      "setTimeout(connectWebSocket, 2000);};\n" // Reconectar después de 3 segundos
      "ws.onerror = (error) => {\n"
      "console.error(\"Error en WebSocket:\", error);};}\n"
     // Enviar comandos al rover
    "function sendCommand(command) {\n"
      "if (ws && ws.readyState === WebSocket.OPEN) {\n"
      "ws.send(command);} else {\n"
      "console.error(\"WebSocket desconectado.\");}}\n"
    // Cambiar la velocidad
    "function changeSpeed(increment) {\n"
      "rover_speed+=increment;\n"
      "rover_speed=Math.max(0, Math.min(4095, rover_speed));\n" // Limitar entre 0 y 4095
      "document.getElementById('rover_speed_display').innerText = rover_speed;\n"
      "fetch(`/setSpeed?value=${rover_speed}`)\n"
      ".then(response => {\n"
      "if (!response.ok) {\n"
      "throw new Error(\"Error al actualizar la velocidad\");}})\n"
      ".catch(error=>console.error(\"Error:\",error));}\n";

const char script_04[1024] PROGMEM =    
    // Iniciar conexión WebSocket
    "connectWebSocket();\n"
    // mover cámara
    "let moveInterval;\n"
    "function startMovingServo1(command) {\n"
      "sendCommand(command);\n" // Iniciar movimiento al presionar el botón
      // Continuar enviando comandos cada 50 ms mientras el botón esté presionado
      "moveInterval = setInterval(() => {\n"
      "sendCommand(command);  }, 50);"
      "}\n"

    "function startMovingServo2(command) {\n"
      "sendCommand(command);\n" // Iniciar movimiento al presionar el botón
      // Continuar enviando comandos cada 100ms mientras el botón esté presionado
      "moveInterval = setInterval(() => {\n"
      "sendCommand(command);},50);}\n"

    "function stopMovingServo1() {\n"
      "clearInterval(moveInterval);\n" // Detener movimiento al soltar el botón
      "sendCommand('camstop');}\n" // Enviar comando de detener el servo

    "function stopMovingServo2() {\n"
      "clearInterval(moveInterval);\n" // Detener movimiento al soltar el botón
      "sendCommand('camstop');}\n"; // Enviar comando de detener el servo

const char script_05[1024] PROGMEM =    
    "function updateIndicators(azimut,elevacion,giro,temp,hum) {"
      "document.getElementById(\"vehicleazimut\").style.transform=`rotate(${azimut}deg)`;"
      "document.getElementById(\"vehicleelevacion\").style.transform=`rotate(${elevacion}deg)`;"
      "document.getElementById(\"vehiclegiro\").style.transform=`rotate(${giro}deg)`;"

      // Actualizar gráficos de temperatura y humedad
      "const tempBarFill=document.querySelector(\"#tempBar .progress-bar-fill\");"
      "const humBarFill=document.querySelector(\"#humBar .progress-bar-fill\");"

      "tempBarFill.style.width=`${(temp/50)*100}%`;"
      "tempBarFill.parentElement.setAttribute(\"data-value\",`${temp.toFixed(1)}°C`);"

      "humBarFill.style.width = `${hum}%`;"
      "humBarFill.parentElement.setAttribute(\"data-value\",`${hum.toFixed(1)}%`);"
      "document.getElementById(\"azim\").innerText=azimut.toFixed(1);"
      "document.getElementById(\"elev\").innerText=elevacion.toFixed(1);"
      "document.getElementById(\"giro\").innerText=giro.toFixed(1);}"
    "</script>\n"
    "</body>\n"
  "</html>\n";

String SendHTML(bool refrescar) {
    // Cabecera de todas las paginas WEB
  String ptr = "";
  ptr += head_1;
  ptr += head_2;
  ptr += head_3;
  ptr += head_4;
  //ptr += head_5;
  ptr += head_6;
  ptr += head_7;
  ptr += head_8;
  //ptr += head_9;
  ptr += head_10;
  ptr += script_01;
  ptr += String(PORT_WEBSOCKET);
  ptr += script_02;
  ptr += script_03;
  ptr += script_04;
  ptr += script_05;
  return ptr;
}

void handle_OnConnect() {
  webserver.send(200, "text/html", SendHTML(true)); 
}

void handle_index() {
  webserver.send(200, "text/html", SendHTML(true));
}

void handle_NotFound() {
  webserver.send(404, "text/plain", "La pagina no existe");
}

void handleSetValue() {
  if (webserver.hasArg("value")) {
    rover_speed = webserver.arg("value").toInt(); // Convertir el valor recibido a entero
    Serial.println(rover_speed);
    }
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_adelante() {
  Serial.println("adelante html");
  rover_adelante();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_atras() {
  rover_atras();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_giroizquierda() {
  rover_giro_izda();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_giroderecha() {
  rover_giro_dcha();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_setspeed() {
  if (webserver.hasArg("value")) {
    String value = webserver.arg("value");
    rover_speed = value.toInt(); // Actualizar la variable rover_speed
    webserver.send(200, "text/plain", "Velocidad actualizada: " + String(rover_speed));
    Serial.println("Velocidad actualizada: " + String(rover_speed)); } 
  else {
    webserver.send(400, "text/plain", "Falta el parámetro 'value'");   }
}

void handle_cochearriba() {
    if (!SPIFFS.exists("/cochearriba.jpg")) {
        webserver.send(404, "text/plain", "Archivo no encontrado en el sistema de archivos");
        return;
    }
    File file = SPIFFS.open("/cochearriba.jpg", "r");
    if (!file) {
        webserver.send(500, "text/plain", "Error al abrir el archivo");
        return;
    }
    webserver.streamFile(file, "image/jpeg");
    file.close();
}

void handle_cochetrasera() {
    if (!SPIFFS.exists("/cochetrasera.jpg")) {
        webserver.send(404, "text/plain", "Archivo no encontrado en el sistema de archivos");
        return;
    }
    File file = SPIFFS.open("/cochetrasera.jpg", "r");
    if (!file) {
        webserver.send(500, "text/plain", "Error al abrir el archivo");
        return;
    }
    webserver.streamFile(file, "image/jpeg");
    file.close();
}

void handle_cochelado() {
    if (!SPIFFS.exists("/cochelado.jpg")) {
        webserver.send(404, "text/plain", "Archivo no encontrado en el sistema de archivos");
        return;
    }
    File file = SPIFFS.open("/cochelado.jpg", "r");
    if (!file) {
        webserver.send(500, "text/plain", "Error al abrir el archivo");
        return;
    }
    webserver.streamFile(file, "image/jpeg");
    file.close();
}

void init_webserver()
{
  webserver.on("/", handle_index); 
  webserver.on("/index.html", handle_index); 
  webserver.on("/set", handleSetValue);
  webserver.on("/adelante", handle_adelante);
  webserver.on("/atras", handle_atras);
  webserver.on("/giroizquierda", handle_giroizquierda);
  webserver.on("/giroderecha", handle_giroderecha);
  webserver.on("/setSpeed", handle_setspeed);
  webserver.on("/cochearriba", handle_cochearriba);
  webserver.on("/cochelado", handle_cochelado);
  webserver.on("/cochetrasera", handle_cochetrasera);

  webserver.onNotFound(handle_NotFound); 
  webserver.begin();
  Serial.println("Servidor HTTP iniciado");
}

