
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

bool wm_nonblocking = false; // change to true to use non blocking

WiFiManager wm; // global wm instance
WiFiManagerParameter custom_field; // global param ( for non blocking w params )

// Función para obtener parámetros desde el servidor
String getParam(String name) {
  if (wm.server->hasArg(name)) {
    return wm.server->arg(name);
    }
  return "";
}

void saveParamCallback(){
  Serial.println(F("[CALLBACK] saveParamCallback Ok"));
  Serial.print(F("PARAM customfieldid = "));
  Serial.println(getParam("customfieldid"));
}

void checkButton(){
  // check for button press
  if ( digitalRead(WIFI_TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(WIFI_TRIGGER_PIN) == LOW ){
      #ifdef DEBUG
        Serial.println(F("Button Pressed"));
      #endif
      delay(3000); // reset delay hold
      if( digitalRead(WIFI_TRIGGER_PIN) == LOW ){
        #ifdef DEBUG
          Serial.println(F("Boton pres.. Borrando conf. reinit"));
        #endif
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      #ifdef DEBUG
        Serial.println(F("Init portal conf"));
      #endif
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("OnDemandAP","password")) {
        #ifdef DEBUG
          Serial.println(F("No se pudo conectar"));
        #endif
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        #ifdef DEBUG
          Serial.println(F("conectado OK"));
        #endif
      }
    }
  }
}

void init_wifi_manager()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  #ifdef DEBUG  
    Serial.println(F("\n Iniciando WiFi"));
  #endif
  pinMode(WIFI_TRIGGER_PIN, INPUT);
  int customFieldLength = 40;
  // test custom html(radio)
  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str); // custom html input
  
  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);

  wm.setClass("invert");
  //set static ip
  wm.setSTAStaticIPConfig(WIFI_IP, WIFI_GW, WIFI_MASK); // set static ip,gw,sn
  wm.setShowStaticFields(true); // force show static ip fields
  wm.setShowDnsFields(true);    // force show dns field always

  wm.setConfigPortalTimeout(120); // auto close configportal after n seconds
  bool res;  
  res = wm.autoConnect(); // auto generated AP name from chipid ESPxxxxx
  //res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
  #ifdef DEBUG 
    if(!res) {
      Serial.println(F("No se pudo conectar"));
      // ESP.restart();
      } 
    else {
      //if you get here you have connected to the WiFi    
      Serial.println(F("conectado OK"));
    }
  #endif
}  

void wifimanager_loop()
{
  if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  checkButton();

}

