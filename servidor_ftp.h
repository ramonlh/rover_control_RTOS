
#include <FTPServer.h>

// Crear el servidor FTP
FTPServer ftpSrv(SPIFFS);

void init_ftp_server()
{
  //bool fsok = SPIFFS.begin();
  ftpSrv.begin(F("admin"), F("12341234"));
}