
#include "./configuration.h"

// ジャンプ時刻が設定されていてまだ飛んでいない場合true
volatile boolean isJumpSeted = false;

// ジャンプ時刻[msec]
volatile unsigned long jumpTime = 0;

void setup() {
  Serial.begin(115200);

  // 光電センサ初期化
  peSensor_Setup();

  // 足位置検出用ホールセンサ初期化
  hallSensor_Setup();

  // デバッグ用ブザー初期化
  buzzer_Setup();

  // ESC初期化
  speedController_Setup();
}

void loop() {
  unsigned long now = millis();
  
  // ジャンプ開始
  if (isJumpSeted && (jumpTime <= now)) {
    isJumpSeted = false;
    Serial.println("Jump!");
  }

  // ホールセンサ変化時
  static boolean hallsens_old = hallSensor_GetState();
  if (hallsens_old != hallSensor_GetState()) {
    buzzer_Start();
    hallsens_old = ! hallsens_old;
  }

  // ブザーの鳴らし始めから100ms経過したらブザーを止める
  if (buzzer_GetState() && (100 <= buzzer_GetBuzzerTime())) {
    buzzer_Stop();
  }
}

// 縄が真下に来る時刻が算出された際に呼び出される
// 引数1 btime: 縄が真下にくるの時刻[msec]( millis() == btimeの時に縄が真下にくる )
// ※本関数は、光電センサ(peSensor)の割込みから呼ばれる
void updateBottomTime(unsigned long btime) {
  jumpTime = btime - jump_START_TIME;
  isJumpSeted = true;
}
