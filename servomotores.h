
#include <Adafruit_PWMServoDriver.h>

// Dirección del PCA9685 (ajusta si es diferente)
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

// Rango de pulsos del servo (ajustar según modelo)
#define SERVO_MIN   50  // Pulso mínimo (~0°)
#define SERVO_MAX  600  // Pulso máximo (~180°)

// Canales del PCA9685 para los servos
#define SERVO1_CHANNEL 4    // I/O 4 del PCA9685
#define SERVO2_CHANNEL 5    // I/O 5 del PCA9685

int angulo_servo_1 = 90;
int angulo_servo_2 = 80;
int ang_lim[8][2] = {{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150}};

void moverServoA(uint8_t canal, int angulo) {

  if (angulo < ang_lim[canal][0]) angulo = ang_lim[canal][0];   // Limitar a 10°
  if (angulo > ang_lim[canal][1]) angulo = ang_lim[canal][1]; // Limitar a 150
  int pulsoservo = SERVO_MIN + ((angulo * (SERVO_MAX - SERVO_MIN)) / 180);
  servos.setPWM(canal, 0, pulsoservo);
}

void task_servomotores(void *pvParameters) {
  servos.begin();
  servos.setPWMFreq(50); // 50Hz para servos
  vTaskDelay(pdMS_TO_TICKS(1000)); 
  Serial.println("Servomotores iniciados");
  // Variable para almacenar el tiempo de la última ejecución
  angulo_servo_1 = 90;
  angulo_servo_2 = 90;

  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1) {
    moverServoA(SERVO1_CHANNEL, angulo_servo_1);
    moverServoA(SERVO2_CHANNEL, angulo_servo_2);
     
    //Serial.println("Task servomotores ejecutada");
    //Esperar hasta que haya pasado 1000 ms desde la última ejecución
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
    
  }
}
