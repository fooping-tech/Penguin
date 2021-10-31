#include "./configuration.h"
#include <Servo.h>

Servo esc;

// ESC初期化処理
void speedController_Setup() {
  esc.attach(speedController_PIN);
  speedController_Stop();
  delay(1000);
}

// モータ停止処理
void speedController_Stop() {
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
  const float k = ((float)speedController_MAX - (float)speedController_MIN)/200.;
  int pulseTime = (int)(k * ((float)percent + 100.)) + speedController_MIN;
  esc.writeMicroseconds(pulseTime);
}
