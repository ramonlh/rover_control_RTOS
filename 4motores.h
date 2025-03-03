
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

void pinModeMux(int pin, int mode)
{
  if (mode == OUTPUT)
    io_mux.pinMode(pin, Mode::Output);
  else
    io_mux.pinMode(pin, Mode::Input);
}

void digitalWriteMux(int pin, int value)
{
  io_mux.pinDigitalWrite(pin, value==0?low:high);
}

void set_dir_motor (int motor, int dir)           // motor: 1-4,  direccion: 0-1
{
  if (dir == 0)
    {
    digitalWriteMux(pin_dir[motor-1][0], LOW);
    digitalWriteMux(pin_dir[motor-1][1], HIGH);
    }
  else
    {
    digitalWriteMux(pin_dir[motor-1][0], HIGH);
    digitalWriteMux(pin_dir[motor-1][1], LOW);
    }
}

void set_motor(int motor, int dir, int speed)  // motor: 1-4, speed: 0-4095
{
  if (speed == 0)
    {
    digitalWriteMux(pin_dir[motor-1][0], HIGH);
    digitalWriteMux(pin_dir[motor-1][1], HIGH);
    speed_motor.setValue(pin_motor[motor-1], 0);
    }
  else
    {
    set_dir_motor(motor,dir);
    speed_motor.setValue(pin_motor[motor-1], speed);
    }
}

void set_speed_rover(int velocidad)
{
  if (velocidad > 4000)
    velocidad = 4000;
  if (velocidad < 600)
    velocidad = 600;
  rover_speed = velocidad;
  for (int i=0; i<4; i++)
    {
    speed_motor.setValue(i,   rover_speed);
    }
}

void init_motores()
{

  Wire.begin(pin_SDA,pin_SCL);    // Initialize I²C
  init_MCP23017();    // Initialize_MCP23017
  init_PCA9685(1600);   // 50 = freq   // Initialize_PCA9685
  
  for (int i=0; i<4; i++)
    {
    speed_motor.setValue(i, 1);
    pinModeMux(pin_dir[i][0], OUTPUT);
    pinModeMux(pin_dir[i][1], OUTPUT);
    digitalWriteMux(pin_dir[i][0], LOW);  // disable
    digitalWriteMux(pin_dir[i][1], LOW);  // disable
    }
  rover_speed = 1000;
}

void rover_stop()
{
    set_motor(1, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(2, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(3, MFORWARD, 0);    // motor, sentido giro, velocidad
    set_motor(4, MFORWARD, 0);    // motor, sentido giro, velocidad
}

void rover_adelante()
{
    set_motor(1, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, MFORWARD, rover_speed);    // motor, sentido giro, velocidad
}

void rover_atras()
{
    set_motor(1, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, MBACK, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, MBACK, rover_speed);    // motor, sentido giro, velocidad
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

