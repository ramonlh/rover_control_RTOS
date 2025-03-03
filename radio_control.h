

#include <RCSwitch.h>

//#define pin_radio_control 32

RCSwitch mySwitch = RCSwitch();

// códigos mando blanco RF433
const int num_buttonsRC = 16;   // número de botones del mando
const unsigned long codesRC[num_buttonsRC] = {6047491, 6047500, 6047536, 6047692, 6047695, 6047548, 6047551, 6047728,
                                     6047683, 6047503, 6047740, 6047731, 6047743, 6047680, 6047539, 6047488};

// Variables para el control del tiempo sin señal
unsigned long ultimoTiempoRC = 0;
const unsigned long tiempoMaxSinRC = 200;  // Tiempo máximo sin señal en ms
int ultimo_boton_RC = 0;  // Último botón presionado                                     

// Función para decodificar el código recibido
int decode_RC(unsigned long codeRC) {
  for (int i = 0; i < num_buttonsRC; i++) {
    if (codesRC[i] == codeRC) {
      return i + 1; // Se devuelve el número de botón real
    }
  }
  return 0; // No encontrado
}

// Función para leer el código RF
int lee_RC() {
  if (mySwitch.available()) {
    unsigned long code = mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    if (code == 0) return 0; // Código inválido
    Serial.println(code);
    return decode_RC(code);
  }
  return 0;
}
void task_radiocontrol(void *pvParameters) 
{  
  mySwitch.enableReceive(pin_radio_control);  
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while (1) { // La tarea nunca debe salir
    int boton_RC = lee_RC();

    if (boton_RC > 0) {
      ultimoTiempoRC = millis();  // Se actualiza el tiempo de última recepción
      ultimo_boton_RC = boton_RC; // Se guarda el último botón presionado

      switch (boton_RC) {
        case 1:
          //rumbo_adelante = 1;
          //rumbo_objetivo = angleZ;
          rover_adelante();
          break;
        case 2:
          rumbo_adelante = 0;
          rover_giro_dcha();
          break;
        case 3:
          rumbo_adelante = 0;
          rover_giro_izda();
          break;
        case 4:
          rover_atras();
          break;
        case 5:
          rumbo_adelante = 0;
          rover_lat_izda();
          break;
        case 6:
          rumbo_adelante = 0;
          rover_lat_dcha();
          break;
        case 7:
          rumbo_adelante = 0;
          rover_atras_izda();
          break;
        case 8:
          rumbo_adelante = 0;
          rover_atras_dcha();
          break;
        case 9:
          rumbo_adelante = 0;
          rover_rot_izda();
          break;
        case 10:
          rumbo_adelante = 0;
          rover_rot_dcha();
          break;
        case 11:
          set_speed_rover(rover_speed - 500);
          break;
        case 12:
          set_speed_rover(rover_speed + 500);
          break;
        case 13:
          digitalWrite(pin_led_7colores, HIGH);
          break;
        case 14:
          digitalWrite(pin_led_7colores, LOW);
          break;
      }
    }

    // Si ha pasado más de "tiempoMaxSinRC" sin recibir señal, detener el rover
    if (millis() - ultimoTiempoRC > tiempoMaxSinRC) {
      if (ultimo_boton_RC != 15 && ultimo_boton_RC != 16) {
        rover_stop();
        rumbo_adelante = 0;
        ultimo_boton_RC = 0;  // Reset de último botón
      }
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
  }
}

