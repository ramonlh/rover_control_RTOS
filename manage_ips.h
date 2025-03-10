

#include <ArduinoJson.h>

unsigned long time_delay60 = 300000;
unsigned long last_time;

#define I_AM_SERVER   // descomentar esto para el programa principal de control rover_control_RTOS.ino
//#define I_AM_CAM   // descomentar esto para el programa de la cámara CameraWebServer.ino

#ifdef I_AM_SERVER
  const char* urlsend PROGMEM = "http://dweet.io/dweet/for/roverdiegoipserver?ip="; 
  const char* urlget  PROGMEM = "http://dweet.io/get/latest/dweet/for/roverdiegoipcam";
#else
  const char* urlsend PROGMEM = "http://dweet.io/dweet/for/roverdiegoipcam?ip=";
  const char* urlget PROGMEM = "http://dweet.io/get/latest/dweet/for/roverdiegoipserver";
#endif


void init_manageips()
  {
  last_time= millis();
  }

void send_IPs()
{
  String url= String(urlsend) + WiFi.localIP().toString();
  HTTPClient http;
  http.begin(url);  //HTTP
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      }
    } 
  else {
    #ifdef DEBUG
      Serial.printf(F("HTTP error: %s\n"), http.errorToString(httpCode).c_str());
    #endif
    }
  http.end();
}

void get_IPs()
{
  String url = String(urlget);
  HTTPClient http;
  http.begin(url);  //HTTP
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
      StaticJsonDocument<200> doc;  // Ajusta el tamaño si el JSON es más grande
      DeserializationError error = deserializeJson(doc, payload.c_str()); // Convertir String a const char*
      if (error) {
        #ifdef DEBUG
          Serial.print(F("Error parse JSON: "));
          Serial.println(error.c_str());
        #endif
        return;
        }
      // Extraer la IP
      const char* ip = doc["with"][0]["content"]["ip"];
      if (ip) {
        strncpy(other_ip, ip, 15);
        other_ip[15] = '\0'; // Asegurar terminación segura        
        } 
      else {
        #ifdef DEBUG
          Serial.println(F("No se encontró la IP"));
        #endif
        }
      }
    } 
  else {
    #ifdef DEBUG
      Serial.printf(F("HTTP error: %s\n"), http.errorToString(httpCode).c_str());
    #endif
    }
  http.end();
}

void send_and_get_ips()
{
  send_IPs();
  get_IPs();
  #ifdef DEBUG
    Serial.print(F("other_ip:")); Serial.println(other_ip);
  #endif
}

void manage_ips_loop()
{
  if ((millis()-last_time) > time_delay60)
    {
    send_and_get_ips();
    last_time=millis();
    }
}