
#include "DHTesp.h" 

//DHTesp dht;
/** Pin number for DHT11 data pin */
//#define pin_DHT11 13

DHTesp dht11;
TempAndHumidity valores_DHT11;

void task_dht11(void *pvParameters) {
    dht11.setup(pin_DHT11, DHTesp::DHT11);
    Serial.println(F("DHT11 Ok"));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1) {
        TempAndHumidity newValues;
        int attempts = 3; // Número de intentos

        while (attempts--) {
            newValues = dht11.getTempAndHumidity();
            if (!isnan(newValues.temperature) && !isnan(newValues.humidity)) {
                valores_DHT11 = newValues; // Guardar valores válidos
                break;  // Salir del loop si la lectura es válida
            }
            //Serial.println(F("DHT11: Error en la lectura, reintentando..."));
            delay(500); // Esperar antes de reintentar
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000)); // Leer cada 10s
    }
}
