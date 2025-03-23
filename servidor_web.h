
#include <UnixTime.h>
#include <web_pages.h>
#include <HTTPClient.h>

WebServer webserver(PORT_WEBSERVER);    // #define port_servidor_web 91

char other_ip[16]; // 

//WiFiClient espClient;

String SendHTML() {
    // Cabecera de todas las paginas WEB
  String ptr = "";
  ptr += head_1;
  ptr += String(other_ip);
  ptr += "192.168.4.2";
  ptr += head_2;
  ptr += script_01;
  return ptr;
}

String SendSetupHTML() {
    // Cabecera de todas las paginas WEB
  String ptr = "";
  ptr += setup_page_1;
  return ptr;
}

void send_200()
{
  webserver.send(200, F("text/html"), SendHTML()); 
}

void send_204()
{
  webserver.send(204);
}

void handle_index() {
  send_200();
}

const char* textplain = "text/plain";

void handle_NotFound() {
  webserver.send(404, textplain, F("La pagina no existe"));
}

void handle_image(const char* filename) {
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    webserver.send(404, textplain, F("Archivo no encontrado"));
    return;
  }
  webserver.streamFile(file, "image/jpeg");
  file.close();
}

void handle_cochearriba() { handle_image("/carup.jpg"); }
void handle_cochetrasera() { handle_image("/carback.jpg"); }
void handle_cochelado() { handle_image("/carside.jpg"); }

void handle_setup() { 
  webserver.send(200, F("text/html"), SendSetupHTML()); 
  }

void init_webserver()
{
  webserver.on("/", handle_index); 
  // carga imagenes
  webserver.on("/carup", handle_cochearriba);
  webserver.on("/carside", handle_cochelado);
  webserver.on("/carback", handle_cochetrasera);
  webserver.on("/setup", handle_setup);
  

  webserver.onNotFound(handle_NotFound); 
  webserver.begin();
}

