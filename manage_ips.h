

long time_delay60 = 10000;
long last_time;

#define IAMSERVER   // descomentar esto para el programa principal de control rover_control_RTOS.ino
//#define IAMCAM   // descomentar esto para el programa de la cámara CameraWebServer.ino

#ifdef IAMSERVER
  const char* urlsend = "http://dweet.io/dweet/for/roverdiegoipserver?ip=";
  const char* urlget = "http://dweet.io/get/latest/dweet/for/roverdiegoipcam";
#else
  const char* urlsend = "http://dweet.io/dweet/for/roverdiegoipcam?ip=";
  const char* urlget = "http://dweet.io/get/latest/dweet/for/roverdiegoipserver";
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
    //Serial.printf("HTTP code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
    }
    } 
  else {
    Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
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
    //Serial.printf("HTTP code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
    }
  } else {
    Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void manage_ips_loop()
{
  if ((millis()-last_time) > time_delay60)
    {
    send_IPs();
    get_IPs();
    last_time=millis();
    }
}