#include "./configuration.h"

void button_Setup() {
  pinMode(button_STARTSTOP_PIN, INPUT_PULLUP);
  pinMode(button_PESENSORDISABLE_PIN, INPUT_PULLUP);
  pinMode(button_FORWARD_PIN, INPUT_PULLUP);
  pinMode(button_BACKWARD_PIN, INPUT_PULLUP);
}

boolean button_Forward() {
  return (digitalRead(button_FORWARD_PIN) == LOW);
}

boolean button_Backward() {
  return (digitalRead(button_BACKWARD_PIN) == LOW);
}

boolean button_StartStop() {
  return (digitalRead(button_STARTSTOP_PIN) == LOW);
}

boolean button_PeSensorDisable() {
  return (digitalRead(button_PESENSORDISABLE_PIN) == LOW);
}
