#include "./configuration.h"

// ブザーの状態(trueの時は鳴っている状態)
boolean buzzer_State;

// ブザーの鳴らし始めた時刻[msec]
unsigned long buzzer_StartTime = 0;

// 現在のブザーの状態(trueの時は鳴っている状態)
boolean buzzer_GetState() {
  return buzzer_State;
}

// 鳴らし始めからの時間[msec]を返す
unsigned long buzzer_GetBuzzerTime() {
  if (buzzer_GetState()) {
    return millis() - buzzer_StartTime;
  }
  return 0;
}

// ブザーの初期化処理
void buzzer_Setup() {
  pinMode(buzzer_PIN, OUTPUT);
  buzzer_Stop();
}

// ブザーを鳴らし始める
void buzzer_Start() {
  if (!buzzer_GetState()) {
    buzzer_StartTime = millis();
    buzzer_State = true;
  }
  digitalWrite(buzzer_PIN, HIGH);
}

// ブザーを止める
void buzzer_Stop() {
  buzzer_State = false;
  digitalWrite(buzzer_PIN, LOW);
}
