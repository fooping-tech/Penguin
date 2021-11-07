#include "./configuration.h"

// 現在設定されているスピード(0% ~ 100%)
int speedController_Speed = 0;

// 現在設定されているスピード(0% ~ 100%)を返す
int speedController_GetSpeed() {
  return speedController_Speed;
}

// 初期化処理
void speedController_Setup() {
  speedController_Stop();
}

// モータ停止処理
void speedController_Stop() {
  speedController_Output(0);
}

// モータ出力設定
// percent: 0～100% で出力指示
void speedController_Output(int percent) {
//  if (speedController_DIR_INVERSE) {
//    percent = -1 * percent;
//  }
  if (percent < 0) {
    percent = 0;
  }
  if (100 < percent) {
    percent = 100;
  }
  speedController_Speed = percent;
  int pwm = (int)(256./100. * (float)percent);
  analogWrite(speedController_PIN, pwm);
  
//  if (speedController_Speed <= 0) {
//    digitalWrite(speedController_PIN, LOW);
//  } else {
//    digitalWrite(speedController_PIN, HIGH);
//  }
//  const float k = ((float)speedController_MAX - (float)speedController_MIN)/200.;
//  int pulseTime = (int)(k * ((float)percent + 100.)) + speedController_MIN;
//  esc.writeMicroseconds(pulseTime);
}
