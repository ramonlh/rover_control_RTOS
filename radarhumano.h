

#include "RCWL0516.h"

RCWL0516 radarhumano(pin_radar);

// Definir la función que se ejecutará en la tarea
void task_radarhumano(void *pvParameters) {
  // Initialize sensor radar
  radarhumano.activate();
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    int value = digitalRead(pin_radar);  
    if (value == HIGH)
     {
     //digitalWrite(pin_led_7colores, HIGH);
     }
   else
     {
     //digitalWrite(pin_led_7colores, LOW);
     }  
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_radar));
  }
}
