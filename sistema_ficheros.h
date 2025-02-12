

#include <FS.h>
#include <SPIFFS.h>  // SPIFFS es el sistema de archivos por defect

void ini_sistema_ficheros()
{
  // Inicializar SPIFFS
  if (!SPIFFS.begin(true)) {  // El parámetro 'true' formatea el sistema de archivos si no está montado
    Serial.println("Error al montar SPIFFS");
    return;
  }
  Serial.println("SPIFFS montado correctamente");
  // Verificar el espacio disponible
  uint32_t totalBytes = SPIFFS.totalBytes();
  uint32_t usedBytes = SPIFFS.usedBytes();
  Serial.print("Total space: ");
  Serial.println(totalBytes);
  Serial.print("Espacio usado: ");
  Serial.print(usedBytes);
  Serial.print("   Libre: ");
  Serial.println(totalBytes - usedBytes);  
}