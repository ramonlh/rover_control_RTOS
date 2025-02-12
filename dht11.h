
#include "DHTesp.h" 

//DHTesp dht;
/** Pin number for DHT11 data pin */
//#define pin_DHT11 13

DHTesp dht11;
TempAndHumidity valores_DHT11;

// Definir la función que se ejecutará en la tarea
void task_dht11(void *pvParameters) {
	dht11.setup(pin_DHT11, DHTesp::DHT11);
	Serial.println("DHT11 iniciado");
  // Variable para almacenar el tiempo de la última ejecución
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    valores_DHT11 = dht11.getTempAndHumidity();
    //Serial.println("DHT11 ejecutada");
    // Esperar hasta que haya pasado 1000 ms desde la última ejecución
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
  }
}

