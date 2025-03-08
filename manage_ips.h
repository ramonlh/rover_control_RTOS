

long time_delay60 = 10000;
long last_time;

#define IAMSERVER
//#define IAMCAM

#ifdef IAMSERVER
  const char* urlsend = "https://dweet.io/dweet/for/roverdiegoipserver?ip=";
  const char* urlget = "https://dweet.io/get/latest/dweet/for/roverdiegoipcam";
#else
  const char* urlsend = "https://dweet.io/dweet/for/roverdiegoipcam?ip=";
  const char* urlget = "https://dweet.io/get/latest/dweet/for/roverdiegoipserver";
#endif

void init_manageips()
  {
  last_time= millis();
  }

void send_IPs()
{
  String url= String(urlsend) + WiFi.localIP().toString();
  Serial.println(url);
  HTTPClient http;
  Serial.print("HTTP GET \n");
  http.begin(url);  //HTTP
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("HTTP code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void get_IPs()
{
  String url = String(urlget);
  HTTPClient http;
  Serial.print("HTTP GET \n");
  http.begin(url);  //HTTP
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("HTTP code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
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
    Serial.println("http");
    send_IPs();
    get_IPs();
    last_time=millis();
    }
}