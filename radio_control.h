#include <RCSwitch.h>

//#define pin_radio_control 32

RCSwitch mySwitch = RCSwitch();

// Códigos mando blanco RF433
const int num_buttonsRC = 16;   // Número de botones del mando
const unsigned long codesRC[num_buttonsRC] = {6047491, 6047500, 6047536, 6047692, 6047695, 6047548, 6047551, 6047728,
                                     6047683, 6047503, 6047740, 6047731, 6047743, 6047680, 6047539, 6047488};

//volatile int tipo_mov = 50; // Estado inicial detenido
volatile unsigned long ultima_senal_RC = 0; // Tiempo de la última señal recibida
//volatile int control_activo = 0; // 0: Ninguno, 1: WebSockets, 2: Radiocontrol

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
    return decode_RC(code);
  }
  return 0;
}

void task_radiocontrol(void *pvParameters) 
{  
  mySwitch.enableReceive(pin_radio_control);  
  TickType_t xLastWakeTime = xTaskGetTickCount();
  Serial.println("RC Ok");
  
  while (1) { // La tarea nunca debe salir
    int boton_RC = lee_RC();
    
    if (boton_RC >= 1 && boton_RC <= 14) {
      //Serial.print("boton:"); Serial.println(boton_RC);
      tipo_mov = 50 + boton_RC; // Asignamos valores de 51 a 60
      ultima_senal_RC = millis(); // Guardamos el tiempo de la última señal
      control_activo = 2; // Se activa el control por RF
      }

    if (boton_RC >= 11 && boton_RC <= 14) {
      switch (boton_RC) {
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
      ultima_senal_RC = millis(); // Actualizamos la última señal recibida para evitar que se pierda el control
      control_activo = 2; // Seguimos en control por RF
    }
    
    // Si han pasado más de 500ms sin recibir señal, se detiene el coche solo si el control activo es RF
    if (millis() - ultima_senal_RC > 200 && control_activo == 2) {
      tipo_mov = 50;
      control_activo = 0;
    }
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(periodo_task_radiocontrol));
  }
}

// Función para manejar WebSockets
void recibir_comando_websocket(int comando) {
  if (comando >= 1 && comando <= 10) {
    tipo_mov = 50 + comando;
    control_activo = 1; // Control por WebSockets activo
  } else if (comando == 0) {
    tipo_mov = 50;
    control_activo = 0;
  }
}
