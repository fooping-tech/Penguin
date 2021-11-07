#include "./configuration.h"


// フォトマイクロセンサ初期化処理
void pmSensor_Setup() {
  pinMode(pmSensor_PIN0, INPUT);
  pinMode(pmSensor_PIN1, INPUT);
}

// 隙間に物がある場合はtrue
boolean pmSensor_GetState0() {
  return (digitalRead(pmSensor_PIN0) == HIGH);
}

// 隙間に物がある場合はtrue
boolean pmSensor_GetState1() {
  return (digitalRead(pmSensor_PIN1) == HIGH);
}
