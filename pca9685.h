
#include <Bonezegei_PCA9685.h>

// Default Address of PCA9685 = 0x40 
Bonezegei_PCA9685 speed_motor(0x40);

void init_PCA9685(int freq)
{
  if (speed_motor.begin()) {
    #ifdef DEBUG
      Serial.println(F("PCA9685 OK"));
    #endif
    // Frequency in Hertz 
    speed_motor.setFrequency(freq);
    } 
  else {
    #ifdef DEBUG
      Serial.println(F("Error:PCA9685"));
    #endif
    }
}

