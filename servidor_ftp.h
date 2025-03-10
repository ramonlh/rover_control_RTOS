
#include <FTPServer.h>

// Crear el servidor FTP
FTPServer ftpSrv(SPIFFS);

void init_ftp_server()
{
  //bool fsok = SPIFFS.begin();
  //Serial.printf_P(PSTR("FS init: %s\n"), fsok ? PSTR("ok") : PSTR("fail!"));
  ftpSrv.begin(F("admin"), F("18921892"));
  #ifdef DEBUG
    Serial.println(F("FTP OK"));
  #endif
}