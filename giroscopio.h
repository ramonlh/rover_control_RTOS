
#include <JY901.h>

float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
float offsetX = 0.0, offsetY = 0.0, offsetZ = 0.0;
float rumbo_objetivo;

/*
// Función para calibrar el giroscopio
void calibrarGiroscopio() {
  int num_muestras = 100;
  float sumX = 0, sumY = 0, sumZ = 0;

  for (int i = 0; i < num_muestras; i++) {
    sumX += JY901.getRoll();
    sumY += JY901.getPitch();
    sumZ += JY901.getYaw();
    vTaskDelay(pdMS_TO_TICKS(20));  // Espera 20 ms entre muestras
  }

  // Calcular el offset promedio
  offsetX = sumX / num_muestras;
  offsetY = sumY / num_muestras;
  offsetZ = sumZ / num_muestras;
}
*/

// Definir la función que se ejecutará en la tarea
void task_giroscopio(void *pvParameters) {
  JY901.startIIC(i2caddress_giroscopio);
  // Realizar calibración antes de iniciar las lecturas
  //calibrarGiroscopio();
  // Variable para almacenar el tiempo de la última ejecución
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    // Código que se ejecutará cada 1000 ms
    angleX = JY901.getRoll();
    angleY = JY901.getPitch();
    angleZ = JY901.getYaw();
    if (angleZ <= 0)
      angleZ = -angleZ;
    else
      angleZ = 360 - angleZ;
    // Esperar hasta que hayan pasado 1000 ms desde la última ejecución
    #ifdef DEBUG
    #endif
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_giroscopio));
  }
}

