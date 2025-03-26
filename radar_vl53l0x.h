

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int ang_radar = 0;
int dist_radar = 0;
int enviar_dist = 0;

int enabled_radar = 0;

void set_pos_radar(int angulo)
{
  angulo_servo_3 = angulo;
  moverServo(SERVO3_CHANNEL, angulo_servo_3);
  if (enabled_radar == 1)
    {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      dist_radar=measure.RangeMilliMeter;
      //Serial.print("Distance: "); Serial.print(dist_radar);Serial.print(" mm   Ángulo: "); Serial.println(angulo_servo_3);
      } 
    else {
      //Serial.println("Out of range");
      }
    }
  else
    {
    dist_radar=0;
    }
  enviar_dist = 1;
}

void task_radar(void *pvParameters) 
{  

  if (!lox.begin()) {
    Serial.println(F("VL53L0X Error"));
    }
  else { 
    enabled_radar = 1;
    Serial.println(F("VL53L0X OK"));
  }
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1) { // La tarea nunca debe salir
    for (int i = 0; i<180; i++)
      {
      ang_radar = i;
      int posservo = (ang_radar * 140 / 180) + 10;
      set_pos_radar(posservo);
      }
    //for (int ang=ang_lim[9][1]; ang>ang_lim[9][0];ang--)
    for (int i = 180; i>0; i--)
      {
      ang_radar = i;
      int posservo = (ang_radar * 140 / 180) + 10;
      set_pos_radar(posservo);
      }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_radar));
  }
}
