

#include <FS.h>
#include <SPIFFS.h>  // SPIFFS es el sistema de archivos por defect

void ini_sistema_ficheros()
{
  // Inicializar SPIFFS
  if (!SPIFFS.begin(true)) {  // El parámetro 'true' formatea el sistema de archivos si no está montado
    Serial.println(F("Error SPIFFS"));
    return;
  }
  Serial.println(F("SPIFFS OK"));
  // Verificar el espacio disponible
  uint32_t totalBytes = SPIFFS.totalBytes();
  uint32_t usedBytes = SPIFFS.usedBytes();
}