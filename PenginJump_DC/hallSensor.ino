#include "./configuration.h"

// ホールセンサ状態
volatile boolean hallSensor_State;

boolean hallSensor_GetState() {
  return hallSensor_State;
}

// ホールセンサの初期設定
void hallSensor_Setup() {
  pinMode(hallSensor_PIN, INPUT_PULLUP);
  hallSensor_State = digitalRead(hallSensor_PIN);
  attachInterrupt(digitalPinToInterrupt(hallSensor_PIN), hallSensor_Change, CHANGE);
}

// ホールセンサ出力変化時の割込み処理
void hallSensor_Change() {
  boolean state = digitalRead(hallSensor_PIN);
  hallSensor_State = state;
}
