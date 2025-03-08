

const char head_1[] PROGMEM =    // cabeceras
  "<!DOCTYPE html>"
  "<html lang=\"es\">"
    "<head>"
      "<meta charset=\"UTF-8\">"
      "<title>Control Rover</title>"
      "<link rel=\"icon\" href=\"data:,\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
      "<style>"
        "body {font-family:Arial, sans-serif; text-align: center; }"
        "button {padding:10px 20px; font-size: 16px; margin: 10px; }"
        "html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
        "body{margin-top:50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"
        ".button {display:inline-block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;"
        "text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n"
        ".button-on {background-color: #3498db;}"
        ".button-on:active {background-color: #2980b9;}"
        ".button-off {background-color: #34495e;}"
        ".button-off:active {background-color: #2c3e50;}"
        "p {font-size: 14px;color: #888;margin-bottom: 10px;}"
        ".indcar {display:flex; justify-content: space-around; margin:10px 0;}"
        ".indtemp {display:flex; justify-content: space-around; margin:10px 0;}"
        ".indicator {text-align: center;}"
        ".vehicle {width: 60px;  height: 60px; transition: transform 0.3s ease; transform-origin: center center;}"
        ".progress-bar {width: 150px; height: 20px; background-color: #e0e0e0; border-radius: 10px; overflow: hidden; position: relative; margin: 10px auto; }"
        ".progress-bar-fill {height: 100%; background-color: #3498db; width: var(--percent, 0%); }"
        ".progress-bar::after {content: attr(data-value); font-size: 14px; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); color: #444444; }"
        /* Media Queries para dispositivos pequeños */
        "@media (max-width: 300px) { button { font-size: 14px; padding: 8px 16px;}"
          ".vehicle {width: 60px; height: 80px;}"
        ".progress-bar {width: 80px;} progress-bar-fill { height: 10px;}"
        "#streamImg {width: 240px;height: 180px;}"
        "h1 {font-size:24px;}"
        "p {font-size:12px;}"
        ".inline-buttons {flex-direction: column; gap: 5px;}}"
        ".semicircles-container {display: flex;justify-content: center;align-items: center;flex-direction:column;margin:20px;}"
        ".semicircle {"
          "position: absolute;width: 80px;height: 40px;border-top-left-radius: 40px;border-top-right-radius: 40px;"
          "background-color: rgba(52,152,219,0.6);opacity:0.4;"
          "transition: width 0.3s,height 0.3s,opacity 0.3s;}"
        ".semicircle:nth-child(1) {animation-delay: 0s;}"
        ".semicircle:nth-child(2) {animation-delay:0.2s;}"
        ".semicircle:nth-child(3) {animation-delay:0.4s;}"
        ".semicircle:nth-child(4) {animation-delay:0.6s;}"
        ".semicircle:nth-child(5) {animation-delay:0.8s;}"     
      "</style>"
    "</head>"
    "<body>"
    "<div class=\"indcar\">"
      "<div class=\"indicator\">"
        "<img src=\"/carup\" alt=\"Car\" id=\"carazim\" class=\"vehicle\">"
        "<p>Rumbo: <span id=\"a\">--</span>°</p>"
      "</div>"
      "<div class=\"indicator\">"
        "<img src=\"/carside\" alt=\"Car\" id=\"carelev\" class=\"vehicle\">"
        "<p>Cabeceo: <span id=\"e\">--</span>°</p>"
      "</div>"
      "<div class=\"indicator\">"
        "<img src=\"/carback\" alt=\"Car\" id=\"cargiro\" class=\"vehicle\">"
        "<p>Alabeo: <span id=\"g\">--</span>°</p>"
      "</div>"
    "</div>"
    "<button id=controlButton1 onmousedown=\"mvServo('camup')\" onmouseup=\"stopServo()\">&#9650;</button><br>"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton2 onmousedown=\"mvServo('caml')\" onmouseup=\"stopServo()\">&#9664;</button>\n</div>"
      "<div><img id=\"streamImg\" src=\"http://192.168.11.92:81/stream\" alt=\"cam\" width=\"320\" height=\"240\" ></div>"
      "<div><button id=controlButton3 onmousedown=\"mvServo('camr')\" onmouseup=\"stopServo()\">&#9654;</button></div>"
    "</div>"
    "<button id=controlButton4 onmousedown=\"mvServo('camdn')\" onmouseup=\"stopServo()\">&#9660;</button><br>\n"
    "<div class=\"semicircles-container\">"
    "  <div class=\"semicircle\" id=\"semiwave1\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave2\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave3\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave4\"></div>"
    "  <div class=\"semicircle\" id=\"semiwave5\"></div>"
    "</div>"
    "<p><span id=\"dUS1\">--</span>cm</p>"
    "<button id=controlButton5 onmousedown=\"mvRover('fl')\" onmouseup=\"stopRover('stop')\">Giro Izda</button>\n"
    "<button id=controlButton6 onmousedown=\"mvRover('f')\" onmouseup=\"stopRover()\">Adelante</button>\n"
    "<button id=controlButton7 onmousedown=\"mvRover('fr')\" onmouseup=\"stopRover()\">Giro Dcha</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton8 onmousedown=\"mvRover('latl')\" onmouseup=\"stopRover()\">Lat Izda</button></div>\n"
      "<div><img src=\"/carup\" alt=\"Car\" id=\"caridle\" class=\"vehicle\"></div>"
      "<div><button id=controlButton9 onmousedown=\"mvRover('latr')\" onmouseup=\"stopRover()\">Lat Dcha</button>\n</div>"
    "</div>"
  "<button id=controlButton10 onmousedown=\"mvRover('rl')\" onmouseup=\"stopRover()\">Atras Izda</button>\n"
  "<button id=controlButton11 onmousedown=\"mvRover('r')\" onmouseup=\"stopRover()\"> Atras  </button>\n"
  "<button id=controlButton12 onmousedown=\"mvRover('rr')\" onmouseup=\"stopRover()\">Atras Der</button><br>\n"
  "<button id=controlButton13 onmousedown=\"mvRover('rotl')\" onmouseup=\"stopRover()\">Rot Izda</button>\n"
  "<button id=controlButton14 onmousedown=\"mvRover('rotr')\" onmouseup=\"stopRover()\">Rot Dcha</button><br>\n"
  // Botones de velocidad en la misma línea -->
  "<div style=\"display: flex; justify-content: center; align-items: center; gap: 10px; margin-top: 20px;\">"
    "<button onclick=\"chsp(-500)\">---</button>\n"

    "<div><span id=\"rover_speed_display\">1000</span></div>"
    "<div id=\"speedometer\" style=\"width: 100px; height: 100px; position: relative;\">"
        "<svg width=\"100\" height=\"100\" viewBox=\"0 0 100 100\">"
          //"<!-- Círculo de fondo -->"
           "<circle cx=\"50\" cy=\"50\" r=\"50\" stroke=\"#ddd\" stroke-width=\"4\" fill=\"none\"></circle>"
          //"<!-- Escala -->"
          "<g id=\"scale\">"
            //"<!-- Crea marcas para la escala -->"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-90 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-60 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(-30 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(0 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(30 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(60 50 50)\"></line>"
          "<line x1=\"50\" y1=\"5\" x2=\"50\" y2=\"15\" stroke=\"#333\" stroke-width=\"2\" transform=\"rotate(90 50 50)\"></line>"
          "</g>"
          "<line id=\"needle\" x1=\"50\" y1=\"50\" x2=\"50\" y2=\"20\" stroke=\"#e74c3c\" stroke-width=\"3\" transform-origin=\"50 50\" transform=\"rotate(0)\"></line>"
          "<text id=\"speedText\" x=\"50\" y=\"70\" text-anchor=\"middle\" font-size=\"12\" fill=\"#444\">1000</text>"
        "</svg>"
      "</div>"
      "<button onclick=\"chsp(+500)\">+++</button>\n"
      "<button id=\"luzButton\" onclick=\"tLUZ()\">LUZ</button>"
  "</div>"   
  "<div style=\"display: flex; justify-content: center; align-items: center; flex-direction: column; margin: 20px;\"></div>"
  "<div id=\"wsStatus\" style=\"font-size: 18px; margin-top: 20px; color: green;\"></div>"
    "<span id=\"t\" style=\"display: none;\">--</span>"
    "<span id=\"h\" style=\"display: none;\">--</span>"
    "<div class=\"indtemp\">"
      "<div class=\"indicator\">Temperatura"
        "<div id=\"tempBar\" class=\"progress-bar\" data-value=\"0%\">"
          "<div class=\"progress-bar-fill\" style=\"--percent: 0%;\"></div>"
        "</div>"
      "</div>"
      "<div class=\"indicator\">Humedad"
        "<div id=\"humBar\" class=\"progress-bar\" data-value=\"0%\">"
          "<div class=\"progress-bar-fill\" style=\"--percent: 0%;\"></div>"
        "</div>"
      "</div>"
    "</div>";

const char script_01[] PROGMEM =
  "<script>"
    "let ws;"
    "let r_sp=1000;" // Velocidad inicial

    // Conectar WebSocket
    "function connectWS() {ws=new WebSocket(`ws://${location.hostname}:81"
    "/`);"
    "ws.onopen = () => {"
      "console.log(\"WS On\");};"
      "document.getElementById(\"wsStatus\").innerText = \"ON\";"
      "document.getElementById(\"wsStatus\").style.color = \"green\";" // Cambia el color a verde

    "ws.onmessage = (event) => {"
      "try {let data = JSON.parse(event.data);"

    // Actualizar dUS1
      "if (data.dUS1 !== undefined) {"
        "document.getElementById(\"dUS1\").innerText = data.dUS1.toFixed(2);" // Mostrar la distancia
        // Controlar las semicircunferencias según la distancia
        "let dist=data.dUS1;"
        // Normalizar la distancia para el rango entre 30 y 200
        "let normalizedDist = Math.max(30, Math.min(dist, 200));" // Mantener dentro de 30-200
        "let waveCount=Math.min(Math.max(Math.floor(normalizedDist / 50), 1), 5);"
        "for (let i = 1; i <= 5; i++) {"
          "let wave = document.getElementById(`semiwave${i}`);"
          "if (i <= waveCount) {"
            "wave.style.opacity=1;" // Mostrar semicircunferencia
            // Ajustar el tamaño de la onda en función de la distancia
            "let sizeFactor=(200-normalizedDist)/170;" // 200 es el valor máximo y 30 es el mínimo
            "wave.style.width=`${40+sizeFactor*60}px`;" // Establecer el tamaño de la onda
            "wave.style.height=`${20+sizeFactor*30}px`;}" 
          "else {"
            "wave.style.opacity = 0;}"
          "}"
        "}"

        "document.getElementById(\"t\").innerText=data.t.toFixed(1);"
        "document.getElementById(\"h\").innerText=data.h.toFixed(1);"
        "document.getElementById(\"a\").innerText=data.a.toFixed(1);"
        "document.getElementById(\"e\").innerText=data.e.toFixed(1);"
        "document.getElementById(\"g\").innerText=data.g.toFixed(1);"
        "document.getElementById(\"dUS1\").innerText=data.dUS1.toFixed(1);"
         // Actualizar indicadores gráficos
        "upInd(data.t,data.h,data.a,data.e,data.g,data.dUS1);"
        "} catch (e) {"
        "console.error(\"Error datos:\", e);"
        "}"
      "};"  
    "ws.onclose = () => {"
      "console.log(\"WS Off\");"
      "document.getElementById(\"wsStatus\").innerText = \"OFF\";"
      "document.getElementById(\"wsStatus\").style.color = \"red\";" // Cambia el color a rojo
      "setTimeout(connectWS, 2000);};" // Reconectar después de 3 segundos
      "ws.onerror = (error) => {"
      "console.error(\"Error WS:\", error);};}"

      "let moveInterval;"

      "function sCom(command) {"
          "if (ws && ws.readyState === WebSocket.OPEN) {"
            "if (command === 'stop') {"
              "clearInterval(moveInterval);" // Si el comando es 'stop', detén el intervalo y envía el comando de parada
              "ws.send(command);}"
            "else"
              "{ws.send(command);}" // Para otros comandos, envíalos directamente
            "}"
          "else {"
            "console.error(\"WS Off\");}"
          "}"

    // Cambiar la velocidad
    "function chsp(increment) {"
      "r_sp+=increment;"
      "r_sp=Math.max(500, Math.min(4000, r_sp));" // Limitar entre 0 y 4095
      "document.getElementById('rover_speed_display').innerText = r_sp;"
      "document.getElementById('speedText').textContent = r_sp;" // Cambiado a textContent

      // Calcula el ángulo para la aguja (0° para velocidad mínima, 180° para velocidad máxima)
      "const maxSp=4095;"
      "const minAng=-90;" // Aguja en 0
      "const maxAng=90;"  // Aguja en velocidad máxima
      "const angle=minAng + (r_sp/maxSp)*(maxAng-minAng);"
      // Actualiza la rotación de la aguja
      "const needle = document.getElementById('needle');"
      "needle.style.transform = `rotate(${angle}deg)`;"

      "fetch(`/setSp?value=${r_sp}`)\n"
        ".then(response => {"
        "if (!response.ok) {"
        "throw new Error(\"Error act. vel.\");}})"
        ".catch(error=>console.error(\"Error:\",error));}"   

    // Iniciar conexión WebSocket
    "connectWS();"

    // mover cámara
    "let mvInt;\n"
    "function mvServo(command) {"
      "sCom(command);\n" // Iniciar movimiento al presionar el botón
      // Continuar enviando comandos cada 50 ms mientras el botón esté presionado
      "mvInt = setInterval(() => {sCom(command);},50);}"

    "function stopServo() {clearInterval(mvInt);sCom('camstop');}" // Enviar comando de detener el servo

    "function mvRover(command) {"
      "sCom(command);\n" // Iniciar movimiento al presionar el botón
      // Continuar enviando comandos cada 50 ms mientras el botón esté presionado
      "mvInt = setInterval(() => {sCom(command);},50);}"

    "function stopRover() {clearInterval(mvInt);sCom('stop');}" // Enviar comando de detener el rover

    "function upInd(t,h,a,e,g) {"
      "document.getElementById(\"carazim\").style.transform=`rotate(${a}deg)`;"
      "document.getElementById(\"carelev\").style.transform=`rotate(${e}deg)`;"
      "document.getElementById(\"cargiro\").style.transform=`rotate(${g}deg)`;"

      // Actualizar gráficos de temperatura y humedad
      "const tBFill=document.querySelector(\"#tempBar .progress-bar-fill\");"
      "const hBFill=document.querySelector(\"#humBar .progress-bar-fill\");"

      "tBFill.style.width=`${(t/50)*100}%`;"
      "tBFill.parentElement.setAttribute(\"data-value\",`${t.toFixed(1)}°C`);"

      "hBFill.style.width = `${h}%`;"
      "hBFill.parentElement.setAttribute(\"data-value\",`${h.toFixed(1)}%`);"
      "document.getElementById(\"a\").innerText=a.toFixed(1);"
      "document.getElementById(\"e\").innerText=e.toFixed(1);"
      "document.getElementById(\"g\").innerText=g.toFixed(1);}"
      "function tLUZ() {fetch('/tLUZ').then(response => response.text()).then(data => console.log(\"Resp. server:\", data)).catch(error => console.error(\"Error:\", error));}"
    "</script>"
    "</body>"
  "</html>";

