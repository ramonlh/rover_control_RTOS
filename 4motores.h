#include "mcp23017.h"
#include <pca9685.h>

#define MFORWARD 1   // motor adelante
#define MBACK 0   // motor adelante
#define MDI 4   // motor delantero izquierdo
#define MTI 2   // motor trasero izquierdo 
#define MDD 3   // motor delantero derecho
#define MTD 1   // motor trasero derecho 

const uint8_t pin_dir[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}}; // pines del mux mcp23017
                          // 0=DI 1=TI 2= DD 3=TD
const uint8_t pin_motor[4] = {0, 1, 2, 3};                      // salidas pwm del mux PCA9685

static Output low = Output::Low;
static Output high = Output::High;

static int rover_speed = 1000;
//int rumbo_adelante=0;

//int speed_rover = 1000;   // para usar con la task, rover_speed se quitará
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

void rover_move(int d1, int d2,  int d3, int d4, int s1, int s2, int s3, int s4) {
  set_motor(1, d1, s1);
  set_motor(2, d2, s2);
  set_motor(3, d3, s3);
  set_motor(4, d4, s4);
}

void handle_obstaculoUS()
  {
  // mirar la distancia del sensor de ultrasonidos
  //int distancia = dUS1;
  int distancia = dist_radar;
  //delay(5);
  //if (rumbo_adelante == 1)
    {
    if (distancia <= 150)
      {
      //Serial.println("parar");
      //rover_stop();
      //rumbo_adelante = 0;
      set_speed_rover(0);    
      digitalWrite(pin_led_7colores, LOW);
      }
    else if (distancia <= 350)
      {
      set_speed_rover(1000);    
      } 
    else if (distancia > 350)
      {
      set_speed_rover(3000); // Velocidad normal        
      }
    }
  }

volatile int control_activo = 0; // 0: Ninguno, 1: WebSockets, 2: Radiocontrol
int enabled_motores = 1;

void task_motores(void *pvParameters) {
  Wire.begin(pin_SDA, pin_SCL);
  Serial.print("MCP23017 "); Serial.println(init_MCP23017()==0?"OK":"ERROR");
  Serial.print("PCA9685 "); Serial.println(init_PCA9685(1600)==0?"OK":"ERROR");

  for (int i = 0; i < 4; i++) {
    speed_motor.setValue(i, 1);
    io_mux.pinMode(pin_dir[i][0], Mode::Output);
    io_mux.pinMode(pin_dir[i][1], Mode::Output);
    digitalWriteMux(pin_dir[i][0], LOW);
    digitalWriteMux(pin_dir[i][1], LOW);
    }

  //speed_rover = 1000;
  tipo_mov = 99;

  vTaskDelay(pdMS_TO_TICKS(1000));
  TickType_t xLastWakeTime = xTaskGetTickCount();

  int last_mov = 0;

  while (1) {
    if (enabled_motores > 0)
      {
      //Serial.println(tipo_mov);
      if (tipo_mov != last_mov) {
        last_mov = tipo_mov;
        }
      // Detener el rover si el tipo_mov es 0 (WebSockets) o 50 (RF cuando no es control activo)
      if ((tipo_mov == 0 || tipo_mov == 50) && control_activo != 2) {
        tipo_mov=99;
        rover_stop();
        } 
      else if ((tipo_mov == 1) || (tipo_mov == 51)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MFORWARD, MFORWARD, MFORWARD, MFORWARD, rover_speed, rover_speed, rover_speed, rover_speed);  // adelante
        } 
      else if ((tipo_mov == 2) || (tipo_mov == 52)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MBACK, MBACK, MBACK, MBACK, rover_speed, rover_speed, rover_speed, rover_speed);  // atras
        } 
      else if ((tipo_mov == 3) || (tipo_mov == 53)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MFORWARD, MFORWARD, MFORWARD, MFORWARD, rover_speed, rover_speed/2, rover_speed, rover_speed/2);  // adelante derecha
        } 
      else if ((tipo_mov == 4) || (tipo_mov == 54)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MFORWARD, MFORWARD, MFORWARD, MFORWARD, rover_speed/2, rover_speed, rover_speed/2, rover_speed);  // adelante izquierda
          } 
      else if ((tipo_mov == 5) || (tipo_mov == 55)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MBACK, MBACK, MBACK, MBACK, rover_speed/2, rover_speed, rover_speed/2, rover_speed);  // atras izquierda
        } 
      else if ((tipo_mov == 6) || (tipo_mov == 56)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MBACK, MBACK, MBACK, MBACK,rover_speed, rover_speed/2, rover_speed, rover_speed/2);  // atras derecha
        } 
      else if ((tipo_mov == 7) || (tipo_mov == 57)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MFORWARD, MBACK, MFORWARD, MBACK,rover_speed, rover_speed, rover_speed, rover_speed);   // rot derecha
        } 
      else if ((tipo_mov == 8) || (tipo_mov == 58)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MBACK, MFORWARD, MBACK, MFORWARD,rover_speed, rover_speed, rover_speed, rover_speed);   // rot izquierda
        } 
      else if ((tipo_mov == 9) || (tipo_mov == 59)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MBACK, MFORWARD, MFORWARD, MBACK, rover_speed, rover_speed, rover_speed, rover_speed);  // lat izquierda
        } 
      else if ((tipo_mov == 10) || (tipo_mov == 60)) {
        if (rover_speed < 500) rover_speed = 1000;
        rover_move(MFORWARD, MBACK, MBACK, MFORWARD, rover_speed, rover_speed, rover_speed, rover_speed);  // lat derecha
        }
      }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_motores));
  }
}
