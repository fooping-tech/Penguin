#include "./configuration.h"

float gearPosition_offset = 0;

void gearPosition_Setup() {
  gearPosition_offset = 180 - gearPosition_GetPosition();
}

float gearPosition_GetPosition() {
  float ad_value = (float)analogRead(gearPosition_POTENTIOMETERS_PIN);
  
  float deg = ad_value / 1024. * 360.;
  deg += gearPosition_offset;
  while (360 < deg) {
    deg -= 360;
  }
  while (deg < 0) {
    deg += 360;
  }
  return deg;
}
