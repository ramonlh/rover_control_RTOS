

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int ang_radar = 0;
int dist_radar = 0;
int enviar_dist = 0;

void set_pos_radar(int angulo)
{
  VL53L0X_RangingMeasurementData_t measure;
  angulo_servo_3 = angulo;
  ang_radar = angulo;
  moverServo(SERVO3_CHANNEL, angulo_servo_3);
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    dist_radar=measure.RangeMilliMeter;
    enviar_dist = 1;
    //Serial.print("Distance: "); Serial.print(dist_radar);Serial.print(" mm   Ángulo: "); Serial.println(angulo_servo_3);
  } else {
    //Serial.println("Out of range");
    }
}

void task_radar(void *pvParameters) 
{  
  if (!lox.begin()) {
    Serial.println(F("Error al iniciar VL53L0X"));
    }
  else { {
    Serial.println(F("VL53L0X OK"));
    }
  }
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1) { // La tarea nunca debe salir
    for (int ang=ang_lim[9][0]; ang<ang_lim[9][1];ang++)
      {
      set_pos_radar(ang);
      }
    for (int ang=ang_lim[9][1]; ang>ang_lim[9][0];ang--)
      {
      set_pos_radar(ang);
      }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_radar));
  }
}
