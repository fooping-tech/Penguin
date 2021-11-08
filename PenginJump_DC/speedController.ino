#include "./configuration.h"

// 現在設定されているスピード(-100% ~ 100%)
int speedController_Speed = 0;

// 現在設定されているスピード(-100% ~ 100%)を返す
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
// percent: -100～100% で出力指示
void speedController_Output(int percent) {
  if (speedController_DIR_INVERSE) {
    percent = -1 * percent;
  }
  if (percent < -100) {
    percent = -100;
  }
  if (100 < percent) {
    percent = 100;
  }
  speedController_Speed = percent;

  float abs_percent = abs(percent);
  int pwm = 256 - (int)(256. / 100. * (float)abs_percent);

  if (percent == 0) {
    analogWrite(speedController_RPWM_PIN, 256);
    analogWrite(speedController_LPWM_PIN, 256);
  }
  if (percent < 0) {
    analogWrite(speedController_RPWM_PIN, pwm);
    analogWrite(speedController_LPWM_PIN, 256);
  }
  if (0 < percent) {
    analogWrite(speedController_RPWM_PIN, 256);
    analogWrite(speedController_LPWM_PIN, pwm);
  }
}
