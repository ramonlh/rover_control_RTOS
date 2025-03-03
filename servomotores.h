
#include <Adafruit_PWMServoDriver.h>

// Dirección del PCA9685 (ajusta si es diferente)
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

// Rango de pulsos del servo (ajustar según modelo)
#define SERVO_MIN   50  // Pulso mínimo (~0°)
#define SERVO_MAX  600  // Pulso máximo (~180°)

// Canales del PCA9685 para los servos
#define SERVO1_CHANNEL 8    // I/O 4 del PCA9685
#define SERVO2_CHANNEL 9    // I/O 5 del PCA9685

int angulo_servo_1 = 90;
int angulo_servo_2 = 90;
int ang_lim[16][2] = {{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},
                      {10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150},{10,150}};

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
  TickType_t xLastWakeTime = xTaskGetTickCount();

  angulo_servo_1 = 90;
  angulo_servo_2 = 90;

  while(1) {
    moverServoA(SERVO1_CHANNEL, angulo_servo_1);
    moverServoA(SERVO2_CHANNEL, angulo_servo_2);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
  }
}
