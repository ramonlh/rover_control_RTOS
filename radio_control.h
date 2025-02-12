

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// códigos mando blanco RF433
const int num_buttonsRC = 16;   // número de botones del mando
const unsigned long codesRC[num_buttonsRC] = {6047491, 6047500, 6047536, 6047692, 6047695, 6047548, 6047551, 6047728,
                                     6047683, 6047503, 6047740, 6047731, 6047743, 6047680, 6047539, 6047488};

// Función para decodificar el código recibido
int decode_RC(unsigned long codeRC) {
  for (int i = 0; i < num_buttonsRC; i++) {
    if (codesRC[i] == codeRC) {
      return i + 1; // Se devuelve el número de botón real
    }
  }
  return 0; // No encontrado
}

/*
int decode_RC(long codeRC)
{
  bool encontrado = false;
  int i = 0;
  while ((i<num_buttonsRC) && (!encontrado))
    {
    encontrado = (codesRC[i] == codeRC);
    i++;
    }
  if (encontrado)
    return i;
  else
    return 0;
}
*/

// Función para leer el código RF
int lee_RC() {
  if (mySwitch.available()) {
    unsigned long code = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    if (code == 0) return 0; // Código inválido
    return decode_RC(code);
  }
  return 0;
}

/*
int lee_RC()
{
  int boton_aux = 0;
  if (mySwitch.available()) {
    long code= mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    boton_aux = decode_RC(code);
    }
  return boton_aux;
}
*/

void task_radiocontrol(void *pvParameters) 
{  
  mySwitch.enableReceive(pin_radio_control);  // Receiver on interrupt 0 => that is pin #2
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (1)
    {                          // A Task shall never return or exit.
    int boton_RC = lee_RC();
    if (boton_RC > 0)
      {
      if (boton_RC == 1)
        {
        rumbo_adelante=1;
        rumbo_objetivo = angleZ;
        rover_adelante();
        }
      else if (boton_RC == 2)
        {
        //rumbo_adelante=2;
        rover_atras();
        }
      else if (boton_RC == 3)
        {
        rumbo_adelante=0;
        rover_giro_izda();
        }    
      else if (boton_RC == 4)
        {
        rumbo_adelante=0;
        rover_giro_dcha();
        }
      else if (boton_RC == 5)
        {
        rumbo_adelante=0;
        rover_rot_izda();
        }    
      else if (boton_RC == 6)
        {
        rumbo_adelante=0;
        rover_rot_dcha();
        }
      else if (boton_RC == 7)
        {
        digitalWrite(pin_led_7colores, HIGH);
        }
      else if (boton_RC == 8)
        {
        digitalWrite(pin_led_7colores, LOW);
        }
      else if (boton_RC == 9)
        {
        //rumbo_adelante=0;
        set_speed_rover(rover_speed-500);
        }    
      else if (boton_RC == 10)
        {
        //rumbo_adelante=0;
        set_speed_rover(rover_speed+500);
        }
      else if ((boton_RC == 15) || (boton_RC == 16))
        {
        rover_stop();
        rumbo_adelante=0;
        }
      }
    //Serial.println("RC leido");
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
    }
}

