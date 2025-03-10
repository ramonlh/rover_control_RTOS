
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
        //"html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
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
        ".car {width: 60px;  height: 60px; transition: transform 0.3s ease; transform-origin: center center;}"
        ".bar {width: 150px; height: 20px; background-color: #e0e0e0; border-radius: 10px; overflow: hidden; position: relative; margin: 10px auto; }"
        ".barfill {height: 100%; background-color: #3498db; width: var(--percent, 0%); }"
        ".bar::after {content: attr(data-value); font-size: 14px; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); color: #444444; }"
        // Media Queries para dispositivos pequeños 
        "@media (max-width: 300px) { button { font-size: 14px; padding: 8px 16px;}"
          ".car {width: 60px; height: 80px;}"
        ".bar {width: 80px;} barfill { height: 10px;}"
        "#streamImg {width: 240px;height: 180px;}"
        "h1 {font-size:24px;}"
        "p {font-size:12px;}"
        ".inline-buttons {flex-direction: column; gap: 5px;}}"

      "</style>"
    "</head>"
    "<body>"
    "<div class=\"indcar\">"
      "<div class=\"indicator\">"
        "<img src=\"/carup\" alt=\"Car\" id=\"carazim\" class=\"car\">"
        "<p>Rumbo: <span id=\"a\">--</span>°</p>"
      "</div>"
      "<div class=\"indicator\">"
        "<img src=\"/carside\" alt=\"Car\" id=\"carelev\" class=\"car\">"
        "<p>Cab: <span id=\"e\">--</span>°</p>"
      "</div>"
      "<div class=\"indicator\">"
        "<img src=\"/carback\" alt=\"Car\" id=\"cargiro\" class=\"car\">"
        "<p>Ala: <span id=\"g\">--</span>°</p>"
      "</div>"
    "</div>"
    "<button onmousedown=\"mvD('camup')\" onmouseup=\"stopD()\">&#9650;</button><br>"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button onmousedown=\"mvD('caml')\" onmouseup=\"stopD()\">&#9664;</button>\n</div>"
      "<div><img id=\"streamImg\" src=\"http://";

  // aquí se inserta la variable other_ip  "192.168.11.92"

const char head_2[] PROGMEM =    // cabeceras
      ":81/stream\" alt=\"cam\" width=\"320\" height=\"240\" ></div>"
      "<div><button onmousedown=\"mvD('camr')\" onmouseup=\"stopD()\">&#9654;</button></div>"
    "</div>"
    "<button onmousedown=\"mvD('camdn')\" onmouseup=\"stopD()\">&#9660;</button><br>\n"
    "<p><span id=\"dUS1\">--</span>cm</p>"
    "<button onmousedown=\"mvD('fl')\" onmouseup=\"stopD('stop')\">Giro Izda</button>\n"
    "<button onmousedown=\"mvD('f')\" onmouseup=\"stopD()\">Adelante</button>\n"
    "<button onmousedown=\"mvD('fr')\" onmouseup=\"stopD()\">Giro Dcha</button><br>\n"
    "<div style=\"display: flex; align-items: center; justify-content: center; gap: 5px;\">"
      "<div><button id=controlButton8 onmousedown=\"mvD('latl')\" onmouseup=\"stopD()\">Lat Izda</button></div>\n"
      "<div><img src=\"/carup\" alt=\"Car\" id=\"caridle\" class=\"car\"></div>"
      "<div><button onmousedown=\"mvD('latr')\" onmouseup=\"stopD()\">Lat Dcha</button>\n</div>"
    "</div>"
  "<button onmousedown=\"mvD('rl')\" onmouseup=\"stopD()\">Atras Izda</button>\n"
  "<button onmousedown=\"mvD('r')\" onmouseup=\"stopD()\"> Atras  </button>\n"
  "<button onmousedown=\"mvD('rr')\" onmouseup=\"stopD()\">Atras Der</button><br>\n"
  "<button onmousedown=\"mvD('rotl')\" onmouseup=\"stopD()\">Rot Izda</button>\n"
  "<button onmousedown=\"mvD('rotr')\" onmouseup=\"stopD()\">Rot Dcha</button><br>\n"
  // Botones de velocidad en la misma línea -->
  "<div style=\"display: flex; justify-content: center; align-items: center; gap: 10px; margin-top: 20px;\">"
    "<button onclick=\"chsp(-500)\">---</button>\n"
    "<div><span id=\"sp_needle\">1000</span></div>"
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
      "<button onclick=\"tLUZ()\">LUZ</button>"
//      "<button id=\"luzButton\" onclick=\"tLUZ()\">LUZ</button>"
  "</div>"   
  "<div style=\"display: flex; justify-content: center; align-items: center; flex-direction: column; margin: 20px;\"></div>"
  "<div id=\"wsStatus\" style=\"font-size: 18px; margin-top: 20px; color: green;\"></div>"
    "<span id=\"t\" style=\"display: none;\">--</span>"
    "<span id=\"h\" style=\"display: none;\">--</span>"
    "<div class=\"indtemp\">"
      "<div class=\"indicator\">Temp"
        "<div id=\"tempBar\" class=\"bar\" data-value=\"0%\">"
          "<div class=\"barfill\" style=\"--percent: 0%;\"></div>"
        "</div>"
      "</div>"
      "<div class=\"indicator\">Hum"
        "<div id=\"humBar\" class=\"bar\" data-value=\"0%\">"
          "<div class=\"barfill\" style=\"--percent: 0%;\"></div>"
        "</div>"
      "</div>"
    "</div>";

const char script_01[] PROGMEM =
  "<script>"
    "let ws;"
    "let r_sp=1000;" // Velocidad inicial

    // Conectar WebSocket
    "function connectWS() {ws=new WebSocket(`ws://${location.hostname}:81/`);"
    "ws.onopen = () => {"
      "console.log(\"WS On\");};"
      "document.getElementById(\"wsStatus\").innerText = \"ON\";"
      "document.getElementById(\"wsStatus\").style.color = \"green\";" // Cambia el color a verde

    "ws.onmessage = (event) => {"
      "try {let data = JSON.parse(event.data);"
        "upText(\"t\", data.t);"
        "upText(\"h\", data.h);"
        "upText(\"a\", data.a);"
        "upText(\"e\", data.e);"
        "upText(\"g\", data.g);"
        "upText(\"dUS1\", data.dUS1);"

         // Actualizar indicadores gráficos
        "upInd(data.t,data.h,data.a,data.e,data.g,data.dUS1);}"
        "catch (e) {console.error(\"Error datos:\", e);}"
      "};"  
    "ws.onclose = () => {"
      "console.log(\"WS Off\");"
      "document.getElementById(\"wsStatus\").innerText = \"OFF\";"
      "document.getElementById(\"wsStatus\").style.color = \"red\";" // Cambia el color a rojo
      "setTimeout(connectWS, 2000);};" // Reconectar después de 3 segundos
      "ws.onerror = (error) => {"
      "console.error(\"Error WS:\", error);};}"

      "function upText(id, value) {"
        "document.getElementById(id).innerText = value.toFixed(1);"
        "}"

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
      "document.getElementById('sp_needle').innerText = r_sp;"
      "document.getElementById('speedText').textContent = r_sp;" // Cambiado a textContent

      // Calcula el ángulo para la aguja (0° para velocidad mínima, 180° para velocidad máxima)
      "const maxSp=4095;"
      "const minAng=-90;" // Aguja en 0
      "const maxAng=90;"  // Aguja en velocidad máxima
      "const angle=minAng + (r_sp/maxSp)*(maxAng-minAng);"
      // Actualiza la rotación de la aguja
      "const needle = document.getElementById('needle');"
      "needle.style.transform = `rotate(${angle}deg)`;"

      // Enviar velocidad por WebSocket en lugar de fetch
      "if (ws && ws.readyState === WebSocket.OPEN) {"
        "ws.send(JSON.stringify({ type:\"speed\", value: r_sp }));}"
      "else"
        "{console.error(\"WS Off\");}"
        "}"

    // Cambiar luz WebSocket en lugar de fetch
    "function tLUZ() {"
      "if (ws && ws.readyState === WebSocket.OPEN) {"
        "ws.send(JSON.stringify({ type: \"toggle_light\" }));}"
      "else" 
        "{console.error(\"WS Off\");}"
      "}"

    // Iniciar conexión WebSocket
    "connectWS();"

    // mover cámara
    "let mvInt;\n"

    "function mvD(command, stopCmd = 'stop') {"
      "sCom(command);"
      "mvInt = setInterval(() => { sCom(command); }, 50);"
      "}"
    "function stopD(stopCmd = 'stop') {"
      "clearInterval(mvInt);"
      "sCom(stopCmd);"
      "}"

    "function upInd(t,h,a,e,g) {"
      "document.getElementById(\"carazim\").style.transform=`rotate(${a}deg)`;"
      "document.getElementById(\"carelev\").style.transform=`rotate(${e}deg)`;"
      "document.getElementById(\"cargiro\").style.transform=`rotate(${g}deg)`;"

      // Actualizar gráficos de temperatura y humedad
      "const tBFill=document.querySelector(\"#tempBar .barfill\");"
      "const hBFill=document.querySelector(\"#humBar .barfill\");"

      "tBFill.style.width=`${(t/50)*100}%`;"
      "tBFill.parentElement.setAttribute(\"data-value\",`${t.toFixed(1)}°C`);"

      "hBFill.style.width = `${h}%`;"
      "hBFill.parentElement.setAttribute(\"data-value\",`${h.toFixed(1)}%`);"
      "document.getElementById(\"a\").innerText=a.toFixed(1);"
      "document.getElementById(\"e\").innerText=e.toFixed(1);"
      "document.getElementById(\"g\").innerText=g.toFixed(1);}"
    "</script>"
    "</body>"
  "</html>";



