#include "./configuration.h"
#include <Servo.h>

Servo esc;

// 現在設定されているスピード(-100% ~ 100%)
int speedController_Speed = 0;

// 現在設定されているスピード(-100% ~ 100%)を返す
int speedController_GetSpeed() {
  return speedController_Speed;
}

// ESC初期化処理
void speedController_Setup() {
  esc.attach(speedController_PIN);
  speedController_Stop();
  delay(1000);
}

// モータ停止処理
void speedController_Stop() {
  speedController_Speed = 0;
  esc.writeMicroseconds(speedController_NEUTRAL);
}

// モータ出力設定
// percent: -100～100% で出力指示
void speedController_Output(int percent) {
  if (percent < -100) {
    percent = -100;
  }
  if (100 < percent) {
    percent = 100;
  }
  speedController_Speed = percent;
  const float k = ((float)speedController_MAX - (float)speedController_MIN)/200.;
  int pulseTime = (int)(k * ((float)percent + 100.)) + speedController_MIN;
  esc.writeMicroseconds(pulseTime);
}
