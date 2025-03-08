#include "mcp23017.h"
#include <pca9685.h>

#define MFORWARD 1   // motor adelante
#define MBACK 0   // motor adelante
#define MDI 4   // motor delantero izquierdo
#define MTI 2   // motor trasero izquierdo 
#define MDD 3   // motor delantero izquierdo
#define MTD 1   // motor trasero izquierdo 

const uint8_t pin_dir[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}}; // pines del mux mcp23017
// 0=DI  1=TI  2= DD    3=TD
const uint8_t pin_motor[4] = {0, 1, 2, 3};                      // salidas pwm del mux PCA9685

static Output low = Output::Low;
static Output high = Output::High;

static int rover_speed = 1000;
int rumbo_adelante=0;

int speed_rover = 1000;   // para usar con la task, rover_speed se quitará
int tipo_mov = 99;     //  f, r, fl,fr,rl,rr,rotl,rotr, latl, latr 

void digitalWriteMux(int pin, int value)
{
  io_mux.pinDigitalWrite(pin, value==0?low:high);
}

void set_dir_motor(int motor, int dir) {
  digitalWriteMux(pin_dir[motor-1][0], (dir == 0) ? LOW : HIGH);
  digitalWriteMux(pin_dir[motor-1][1], (dir == 0) ? HIGH : LOW);
}

void set_motor(int motor, int dir, int speed) {
  if (speed == 0) {   // stop
    digitalWriteMux(pin_dir[motor-1][0], HIGH);
    digitalWriteMux(pin_dir[motor-1][1], HIGH);
    speed_motor.setValue(pin_motor[motor-1], 0);
    } 
  else {
    set_dir_motor(motor, dir);
    speed_motor.setValue(pin_motor[motor-1], speed);
  }
}

void set_speed_rover(int velocidad) {
  rover_speed = constrain(velocidad, 500, 4000);
  for (int i = 0; i < 4; i++) {
    speed_motor.setValue(i, rover_speed);
  }
}

void rover_stop() {
  for (int i = 1; i <= 4; i++) {
    set_motor(i, MFORWARD, 0);
  }
}

void rover_move(int direction) {
  for (int i = 1; i <= 4; i++) {
    set_motor(i, direction, rover_speed);
  }
}

void rover_adelante() {
  rover_move(MFORWARD);
}

void rover_atras() {
  rover_move(MBACK);
}

void rover_giro_dcha()
{
    set_motor(MDI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(MTD, MFORWARD, 0);    // motor, sentido giro, velocidad
}

void rover_giro_izda()
{
    set_motor(MDI, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(MTI, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(MDD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
}

void rover_atras_dcha()
{
    set_motor(MDI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MBACK, 0);    // motor, sentido giro, velocidad
    set_motor(MTD, MBACK, 0);    // motor, sentido giro, velocidad
}

void rover_atras_izda()
{
    set_motor(MDI, MBACK, 0);    // motor, sentido giro, velocidad
    set_motor(MTI, MBACK, 0);    // motor, sentido giro, velocidad
    set_motor(MDD, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MBACK, rover_speed);    // motor, sentido giro, velocidad
}

void rover_rot_izda()
{
    set_motor(MDI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
}

void rover_rot_dcha()
{
    set_motor(MDI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MBACK, rover_speed);    // motor, sentido giro, velocidad
}

void rover_lat_izda()
{
    set_motor(MDI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MBACK, rover_speed);    // motor, sentido giro, velocidad
}

void rover_lat_dcha()
{
    set_motor(MDI, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTI, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MDD, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(MTD, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
}

void conservar_rumbo()
{
  if (rumbo_objetivo < angleZ)
    {
      set_motor(1, 1, rover_speed-600);  
      set_motor(2, 1, rover_speed-600);  
      delay(500);
      set_motor(1, 1, rover_speed);  
      set_motor(2, 1, rover_speed);        // girar a la izquierda
    }
  else if (rumbo_objetivo > angleZ)
    {
       // girar a la derecha
      set_motor(3, 1, rover_speed-600);  
      set_motor(4, 1, rover_speed-600);  
      delay(500);
      set_motor(3, 1, rover_speed);  
      set_motor(4, 1, rover_speed);        // girar a la izquierda
    }
}

volatile int control_activo = 0; // 0: Ninguno, 1: WebSockets, 2: Radiocontrol

void task_motores(void *pvParameters) {
  Wire.begin(pin_SDA, pin_SCL);
  init_MCP23017();
  init_PCA9685(1600);

  for (int i = 0; i < 4; i++) {
    speed_motor.setValue(i, 1);
    io_mux.pinMode(pin_dir[i][0], Mode::Output);
    io_mux.pinMode(pin_dir[i][1], Mode::Output);
    digitalWriteMux(pin_dir[i][0], LOW);
    digitalWriteMux(pin_dir[i][1], LOW);
  }

  speed_rover = 1000;
  tipo_mov = 99;

  vTaskDelay(pdMS_TO_TICKS(1000));
  TickType_t xLastWakeTime = xTaskGetTickCount();

  int last_mov = 0;

  while (1) {
    if (tipo_mov != last_mov) {
      last_mov = tipo_mov;
    }
    //Serial.println(tipo_mov);
    // Detener el rover si el tipo_mov es 0 (WebSockets) o 50 (RF cuando no es control activo)
    if ((tipo_mov == 0 || tipo_mov == 50) && control_activo != 2) {
      rover_stop();
    } else if ((tipo_mov == 1) || (tipo_mov == 51)) {
      rover_adelante();
    } else if ((tipo_mov == 2) || (tipo_mov == 52)) {
      rover_atras();
    } else if ((tipo_mov == 3) || (tipo_mov == 53)) {
      rover_giro_izda();
    } else if ((tipo_mov == 4) || (tipo_mov == 54)) {
      rover_giro_dcha();
    } else if ((tipo_mov == 5) || (tipo_mov == 55)) {
      rover_atras_izda();
    } else if ((tipo_mov == 6) || (tipo_mov == 56)) {
      rover_atras_dcha();
    } else if ((tipo_mov == 7) || (tipo_mov == 57)) {
      rover_rot_izda();
    } else if ((tipo_mov == 8) || (tipo_mov == 58)) {
      rover_rot_dcha();
    } else if ((tipo_mov == 9) || (tipo_mov == 59)) {
      rover_lat_izda();
    } else if ((tipo_mov == 10) || (tipo_mov == 60)) {
      rover_lat_dcha();
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
  }
}

/*
void task_motores(void *pvParameters) {
  Wire.begin(pin_SDA, pin_SCL);
  init_MCP23017();
  init_PCA9685(1600);

  for (int i = 0; i < 4; i++) {
    speed_motor.setValue(i, 1);
    io_mux.pinMode(pin_dir[i][0], Mode::Output);
    io_mux.pinMode(pin_dir[i][1], Mode::Output);
    digitalWriteMux(pin_dir[i][0], LOW);
    digitalWriteMux(pin_dir[i][1], LOW);
  }

  speed_rover = 1000;
  tipo_mov = 99;

  vTaskDelay(pdMS_TO_TICKS(1000));
  TickType_t xLastWakeTime = xTaskGetTickCount();

  int last_mov = 0;

  while (1) {
    //Serial.print("tipo_mov:"); Serial.println(tipo_mov);

    if (tipo_mov != last_mov) {
      last_mov = tipo_mov;
    }

    // Solo detener si el control es WebSockets o ninguno
    if ((tipo_mov == 50) && (control_activo != 2)) {
      rover_stop();
    } else if ((tipo_mov == 1) || (tipo_mov == 51)) {
      rover_adelante();
    } else if ((tipo_mov == 2) || (tipo_mov == 52)) {
      rover_atras();
    } else if ((tipo_mov == 3) || (tipo_mov == 53)) {
        rover_giro_izda();
      } else if ((tipo_mov == 4) || (tipo_mov == 54)) {
        rover_giro_dcha();
      } else if ((tipo_mov == 5) || (tipo_mov == 55)) {
        rover_atras_izda();
      } else if ((tipo_mov == 6) || (tipo_mov == 56)) {
        rover_atras_dcha();
      } else if ((tipo_mov == 7) || (tipo_mov == 57)) {
        rover_rot_izda();
      } else if ((tipo_mov == 8) || (tipo_mov == 58)) {
        rover_rot_dcha();
      } else if ((tipo_mov == 9) || (tipo_mov == 59)) {
        rover_lat_izda();
      } else if ((tipo_mov == 10) || (tipo_mov == 60)) {
        rover_lat_dcha();
      }

      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
    }
  }
*/