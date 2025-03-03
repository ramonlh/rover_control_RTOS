

const int US1_EchoPin = pin_US1_receptor;   // 34
const int US1_TriggerPin = pin_US1_emisor;  // 33

long distanciaUS1;

// Definir la función que se ejecutará en la tarea
void task_ultrasonidos(void *pvParameters) {
  pinMode(US1_TriggerPin, OUTPUT);
  pinMode(US1_EchoPin, INPUT);
  Serial.println("Sensor ultrasonidos OK");

  // Variables para el filtro de promedio móvil
  const int numReadings = 3;
  int readings[numReadings];
  int readIndex = 0;
  long total = 0;

  // Inicializar el array de lecturas
  for (int i = 0; i < numReadings; i++) { readings[i] = 0; }

  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1) {
    long duration;

    // Generar el pulso de trigger
    digitalWrite(US1_TriggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(US1_TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(US1_TriggerPin, LOW);
    // Medir la duración del eco
    duration = pulseIn(US1_EchoPin, HIGH, 10000); // Timeout de 10 ms
    // Calcular la distancia
    if (duration > 0) {
      long distance = duration * 10 / 292 / 2; // Convertir a distancia en cm
      //Serial.println(distance);
      // Aplicar filtro de promedio móvil
       total -= readings[readIndex];
      readings[readIndex] = distance;
      total += readings[readIndex];
      readIndex = (readIndex + 1) % numReadings;

      distanciaUS1 = total / numReadings; // Distancia filtrada
      } 
    else {
      distanciaUS1 = -1; // Valor inválido si no se detecta eco
      }
    //Serial.println(distanciaUS1);
    // Esperar hasta que haya pasado 100 ms desde la última ejecución
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }
}
/**
void task_ultrasonidosOLD(void *pvParameters) {
  pinMode(US1_TriggerPin, OUTPUT);
  pinMode(US1_EchoPin, INPUT);
  Serial.println("Sensor ultrasonidos OK");
  // Variable para almacenar el tiempo de la última ejecución
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    long duration;

    digitalWrite(US1_TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
    delayMicroseconds(4);
    digitalWrite(US1_TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
    delayMicroseconds(10);
    digitalWrite(US1_TriggerPin, LOW);

//    duration = pulseIn(US_EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
    duration = pulseIn(US1_EchoPin, HIGH, 100);  //medimos el tiempo entre pulsos, en microsegundos
    distanciaUS1 = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
    //Serial.println("Task ultrasonidos ejecutada");
    // Esperar hasta que haya pasado 1000 ms desde la última ejecución
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }
}
**/

void handle_obstaculoUS()
  {
  // mirar la distancia del sensor de ultrasonidos
  int distancia = distanciaUS1;
  //delay(5);
  //Serial.println(distancia);
  if (rumbo_adelante == 1)
    {
    if (distancia <= 15)
      {
      Serial.println("distancia < 10");
      rover_stop();
      rumbo_adelante = 0;
      digitalWrite(pin_led_7colores, LOW);
      }
    else if (distancia <= 35)
      {
      Serial.println("distancia  30");
      set_speed_rover(1000);    
      } 
    else if (distancia > 35 || distancia == -1)
      {
      Serial.println("Sin obstáculos cercanos");
      set_speed_rover(3000); // Velocidad normal        
      }
    }
  }

