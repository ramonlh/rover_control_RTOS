
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

const char head_3[1200] PROGMEM =
        ".indicatorsveh { display:flex; justify-content: space-around; margin:10px 0;}"
        ".indicatorstemp { display:flex; justify-content: space-around; margin:10px 0;}"
        ".indicator { text-align: center;}"
        ".vehicle {width: 60px;  height: 60px; transition: transform 0.3s ease; transform-origin: center center;}"
        ".progress-bar {width: 150px; height: 20px; background-color: #e0e0e0; border-radius: 10px; overflow: hidden; position: relative; margin: 10px auto; }"
        ".progress-bar-fill {height: 100%; background-color: #3498db; width: var(--percent, 0%); }"
        ".progress-bar::after {content: attr(data-value); font-size: 14px; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); color: #444444; }"
        /* Media Queries para dispositivos pequeños */
        "@media (max-width: 300px) { button { font-size: 14px; padding: 8px 16px;}"
          ".vehicle { width: 60px; height: 80px;}"
        ".progress-bar { width: 80px;} progress-bar-fill { height: 10px;}"
        "#streamImg {width: 240px;height: 180px;}"
        "h1 {font-size: 24px;}"
        "p {font-size: 12px;}"
        ".inline-buttons {flex-direction: column; gap: 5px;}}";

const char head_3_1[1024] PROGMEM =  
        ".semicircles-container {"
          "display: flex;"
          "justify-content: center;"
          "align-items: center;"
          "flex-direction: column;"
          "margin: 20px;"
        "}"

        ".semicircle {"
          "position: absolute;"
          "width: 80px;"
          "height: 40px;"
          "border-top-left-radius: 40px;"
          "border-top-right-radius: 40px;"
          "background-color: rgba(52, 152, 219, 0.6);"
          "opacity: 0.4;"
          "transition: width 0.3s, height 0.3s, opacity 0.3s;"
        "}"

        ".semicircle:nth-child(1) {"
          "animation-delay: 0s;"
        "}"
        
        ".semicircle:nth-child(2) {"
          "animation-delay: 0.2s;"
        "}"

        ".semicircle:nth-child(3) {"
          "animation-delay: 0.4s;"
        "}"

        ".semicircle:nth-child(4) {"
          "animation-delay: 0.6s;"
        "}"

        ".semicircle:nth-child(5) {"
          "animation-delay: 0.8s;"
        "}";

const char head_3_2[256] PROGMEM =       
      "</style>\n"
    "</head>\n"
    "<body>\n"
    "<div id=\"wsStatus\" style=\"font-size: 18px; margin-top: 20px; color: green;\"></div>\n";

const char head_4[512] PROGMEM =   // barras de temperatura y humedad
    "<span id=\"temp\" style=\"display: none;\">--</span>\n"
    "<span id=\"hum\" style=\"display: none;\">--</span>\n"
    "<div class=\"indicatorstemp\">\n"
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

const char head_6[512] PROGMEM =    // imágenes de posición del coche
    "<div class=\"indicatorsveh\">\n"
      "<div class=\"indicator\">\n"
        "<img src=\"/cochearriba\" alt=\"Vehiculo\" id=\"vehicleazimut\" class=\"vehicle\">"
        "<p>Rumbo: <span id=\"azim\">--</span>°</p>"
      "</div>\n"
      "<div class=\"indicator\">"
        "<img src=\"/cochelado\" alt=\"Vehiculo\" id=\"vehicleelevacion\" class=\"vehicle\">"
        "<p>Cabeceo: <span id=\"elev\">--</span>°</p>"
      "</div>\n"
      "<div class=\"indicator\">"
        "<img src=\"/cochetrasera\" alt=\"Vehiculo\" id=\"vehiclegiro\" class=\"vehicle\">"
        "<p>Alabeo: <span id=\"giro\">--</span>°</p>"
      "</div>\n"
    "</div>\n";      

const char head_7[1024] PROGMEM =    // botones de movimiento cámara
    "<button id=controlButton1 onmousedown=\"startMovingServo2('camarriba')\" onmouseup=\"stopMovingServo2()\">&#9650;</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton2 onmousedown=\"startMovingServo1('camizda')\" onmouseup=\"stopMovingServo1()\">&#9664;</button>\n</div>\n"
      "<div>"
        "<img id=\"streamImg\" src=\"http://192.168.11.120:81/stream\" alt=\"cam\" width=\"320\" height=\"240\" >\n" //imagen de la cámara
      "</div>\n"
      "<div>\n"
        "<button id=controlButton3 onmousedown=\"startMovingServo1('camdcha')\" onmouseup=\"stopMovingServo1()\">&#9654;</button>\n"
      "</div>"
    "</div>\n"
    "<button id=controlButton4 onmousedown=\"startMovingServo2('camabajo')\" onmouseup=\"stopMovingServo2()\">&#9660;</button><br>\n";

const char head_8[1512] PROGMEM =   // botones movimiento coche
    "<div class=\"semicircles-container\">"
    "  <div class=\"semicircle\" id=\"semiwave1\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave2\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave3\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave4\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave5\"></div>"
    "</div>"
    "<p><span id=\"distUS1\">--</span>cm</p>"
    "<button id=controlButton5 onmousedown=\"sendCommand('giroizda')\" onmouseup=\"sendCommand('stop')\">Giro Izda</button>\n"
    "<button id=controlButton6 onmousedown=\"sendCommand('adelante')\" onmouseup=\"sendCommand('stop')\">Adelante</button>\n"
    "<button id=controlButton7 onmousedown=\"sendCommand('girodcha')\" onmouseup=\"sendCommand('stop')\">Giro Dcha</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton8 onmousedown=\"sendCommand('latizda')\" onmouseup=\"sendCommand('stop')\">Lat Izda</button></div>\n"
      "<div>"
        "<img src=\"/cochearriba\" alt=\"Vehiculo\" id=\"vehicleinutil\" class=\"vehicle\">"  // aquí va la imagen de la cámara
      "</div>\n"
      "<div>\n"
        "<button id=controlButton9 onmousedown=\"sendCommand('latdcha')\" onmouseup=\"sendCommand('stop')\">Lat Dcha</button>\n"
      "</div>"
    "</div>\n"
    "<button id=controlButton10 onmousedown=\"sendCommand('atrasizda')\" onmouseup=\"sendCommand('stop')\">Atras Izda</button>\n"
    "<button id=controlButton11 onmousedown=\"sendCommand('atras')\" onmouseup=\"sendCommand('stop')\"> Atras  </button>\n"
    "<button id=controlButton12 onmousedown=\"sendCommand('atrasdcha')\" onmouseup=\"sendCommand('stop')\">Atras Der</button><br>\n";

//const char head_9[512] PROGMEM ="";

const char head_10[2048] PROGMEM =
  "<button id=controlButton13 onmousedown=\"sendCommand('rotizda')\" onmouseup=\"sendCommand('stop')\">Rot Izda</button>\n"
  "<button id=controlButton14 onmousedown=\"sendCommand('rotdcha')\" onmouseup=\"sendCommand('stop')\">Rot Dcha</button><br>\n"
  // Botones de velocidad en la misma línea -->
  "<div style=\"display: flex; justify-content: center; align-items: center; gap: 10px; margin-top: 20px;\">"
    "<button onclick=\"changeSpeed(-500)\">---</button>\n"

    "<div>\n"
      "<span id=\"rover_speed_display\">2000</span>\n"
    "</div>\n"
    "<div id=\"speedometer\" style=\"width: 100px; height: 100px; position: relative;\">"
        "<svg width=\"100\" height=\"100\" viewBox=\"0 0 100 100\">"
          "<!-- Círculo de fondo -->"
          "<circle cx=\"50\" cy=\"50\" r=\"50\" stroke=\"#ddd\" stroke-width=\"4\" fill=\"none\"></circle>"
              "<!-- Escala -->"
          "<g id=\"scale\">"
            "<!-- Crea marcas para la escala -->"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-90 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-60 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-30 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(0 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(30 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(60 50 50)\"></line>"
            "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(90 50 50)\"></line>"
          "</g>"
          "<!-- Aguja -->"
          "<line id=\"needle\" x1=\"50\" y1=\"50\" x2=\"50\" y2=\"20\" stroke=\"#e74c3c\" stroke-width=\"3\" transform-origin=\"50 50\" transform=\"rotate(0)\"></line>"
          "<text id=\"speedText\" x=\"50\" y=\"70\" text-anchor=\"middle\" font-size=\"12\" fill=\"#444\">2000</text>\""
        "</svg>"
      "</div>"
      "<button onclick=\"changeSpeed(+500)\">+++</button>\n"
      "<button id=\"luzButton\" onclick=\"toggleLUZ()\">LUZ</button>"
  "</div>\n";

const char head_10_1[1580] PROGMEM =   "<div style=\"display: flex; justify-content: center; align-items: center; flex-direction: column; margin: 20px;\">"
      
    "</div>";


const char script_01[128] PROGMEM =
  "<script>\n"
    "let ws;\n"
    "let rover_speed=2048;\n" // Velocidad inicial
    // Conectar WebSocket
    "function connectWebSocket() {\n"
      "ws=new WebSocket(`ws://${location.hostname}:";

const char script_02[2048] PROGMEM =    
    "/`);\n"
    "ws.onopen = () => {\n"
      "console.log(\"WebSocket conectado\");};\n"
      "document.getElementById(\"wsStatus\").innerText = \"Control Rover: Conectado\";"
      "document.getElementById(\"wsStatus\").style.color = \"green\";" // Cambia el color a verde


    "ws.onmessage = (event) => {"
      "console.log(\"Datos recibidos:\", event.data);"
      "try {"
        "let data = JSON.parse(event.data);"

    // Actualizar distUS1
      "if (data.distUS1 !== undefined) {"
        "document.getElementById(\"distUS1\").innerText = data.distUS1.toFixed(2);" // Mostrar la distancia
        // Controlar las semicircunferencias según la distancia
        "let dist = data.distUS1;"
        // Normalizar la distancia para el rango entre 30 y 200
        "let normalizedDist = Math.max(30, Math.min(dist, 200));" // Mantener dentro de 30-200
        "let waveCount = Math.min(Math.max(Math.floor(normalizedDist / 50), 1), 5);"
        //"let waveCount = Math.min(Math.max(Math.floor(dist / 50), 1), 5);" // 1 a 5 semicircunferencias
        "for (let i = 1; i <= 5; i++) {"
          "let wave = document.getElementById(`semiwave${i}`);"
          "if (i <= waveCount) {"
            "wave.style.opacity = 1;" // Mostrar semicircunferencia
            // Ajustar el tamaño de la onda en función de la distancia
            "let sizeFactor = (200 - normalizedDist) / 170;" // 200 es el valor máximo y 30 es el mínimo
            "wave.style.width = `${40 + sizeFactor * 60}px`;" // Establecer el tamaño de la onda
            "wave.style.height = `${20 + sizeFactor * 30}px`;" // Establecer el tamaño de la onda
            "} else {"
              "wave.style.opacity = 0;" // Semicircunferencia oculta
            "}"
          "}"
        "}"

        "document.getElementById(\"azim\").innerText = data.azim.toFixed(1);"
        "document.getElementById(\"elev\").innerText = data.elev.toFixed(1);"
        "document.getElementById(\"giro\").innerText = data.giro.toFixed(1);"
        "document.getElementById(\"temp\").innerText = data.temp.toFixed(1);"
        "document.getElementById(\"hum\").innerText = data.hum.toFixed(1);"
        "document.getElementById(\"distUS1\").innerText = data.distUS1.toFixed(1);"
         // Actualizar indicadores gráficos
        "updateIndicators(data.azim, data.elev, data.giro, data.temp, data.hum);"
        "} catch (e) {"
        "console.error(\"Error al procesar datos:\", e);"
        "}"
      "};";
       
const char script_03[1580] PROGMEM =    
    "ws.onclose = () => {\n"
      "console.log(\"WebSocket desconectado\");\n"
      "document.getElementById(\"wsStatus\").innerText = \"Control Rover: Desconectado\";"
      "document.getElementById(\"wsStatus\").style.color = \"red\";" // Cambia el color a rojo
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
      "rover_speed=Math.max(500, Math.min(4000, rover_speed));\n" // Limitar entre 0 y 4095
      "document.getElementById('rover_speed_display').innerText = rover_speed;\n"
      "document.getElementById('speedText').textContent = rover_speed;" // Cambiado a textContent

      // Calcula el ángulo para la aguja (0° para velocidad mínima, 180° para velocidad máxima)
      "const maxSpeed = 4095;"
      "const minAngle = -90;" // Aguja en 0
      "const maxAngle = 90;"  // Aguja en velocidad máxima
      "const angle = minAngle + (rover_speed / maxSpeed) * (maxAngle - minAngle);"
      // Actualiza la rotación de la aguja
      "const needle = document.getElementById('needle');"
      "needle.style.transform = `rotate(${angle}deg)`;"

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

const char script_05[1278] PROGMEM =    
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
      "function toggleLUZ() {"
          "fetch('/toggleLUZ')"
          ".then(response => response.text())"
          ".then(data => console.log(\"Respuesta del servidor:\", data))"
          ".catch(error => console.error(\"Error:\", error));}"
    "</script>\n"
    "</body>\n"
  "</html>\n";

String SendHTML(bool refrescar) {
    // Cabecera de todas las paginas WEB
  String ptr = "";
  ptr += head_1;
  ptr += head_2;
  ptr += head_3;
  ptr += head_3_1;
  ptr += head_3_2;
  //ptr += head_5;
  ptr += head_6;
  ptr += head_7;
  ptr += head_8;
  //ptr += head_9;
  ptr += head_10;   // control de velocidad
  ptr += head_10_1;   // control de velocidad
  ptr += head_4;    // sensores temperatura y humedad
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

void handleToggleLUZ() {
    digitalWrite(pin_led_7colores, !digitalRead(pin_led_7colores)); // Cambia el estado del pin
    webserver.send(200, "text/plain", "LUZ toggled");
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

void handle_atrasizquierda() {
  rover_atras_izda();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_atrasderecha() {
  rover_atras_dcha();
  webserver.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_setspeed() {
  if (webserver.hasArg("value")) {
    String value = webserver.arg("value");
    rover_speed = value.toInt(); // Actualizar la variable rover_speed
    webserver.send(200, "text/plain", "Velocidad actualizada: " + String(rover_speed));
    //Serial.println("Velocidad actualizada: " + String(rover_speed)); 
    } 
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
  webserver.on("/atrasizquierda", handle_giroizquierda);
  webserver.on("/atrasderecha", handle_giroderecha);
  webserver.on("/setSpeed", handle_setspeed);
  webserver.on("/cochearriba", handle_cochearriba);
  webserver.on("/cochelado", handle_cochelado);
  webserver.on("/cochetrasera", handle_cochetrasera);
  webserver.on("/toggleLUZ", handleToggleLUZ);

  webserver.onNotFound(handle_NotFound); 
  webserver.begin();
  Serial.println("Servidor HTTP iniciado");
}

