#include "./configuration.h"

pmSensor_POSITION pmSensor_Position = POSITION_UNKNOWN;

// 位置が最後に更新された時刻[usec]
unsigned long pmSensor_UpdateTime = 0;

// フォトマイクロセンサ初期化処理
void pmSensor_Setup() {
  pinMode(pmSensor_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pmSensor_PIN), pmSensor_Falling, FALLING);

  // 電源ON時は位置不明
  pmSensor_Position = POSITION_UNKNOWN;
}

void pmSensor_SetPosition(pmSensor_POSITION position) {
  unsigned long now = micros();
  // チャタリング防止のため、1000usec以内の状態変化は無視する
  if ((pmSensor_Position == POSITION_UNKNOWN) || 1000 < (now - pmSensor_UpdateTime)) {
    pmSensor_Position = position;
    pmSensor_UpdateTime = now;
  }
}

pmSensor_POSITION pmSensor_GetPosition() {
  return pmSensor_Position;
}

void pmSensor_Falling() {
  pmSensor_POSITION position = pmSensor_Position;
  if (position != POSITION_UNKNOWN) {
    // 正回転中
    if (0 < speedController_GetSpeed()) {
      if (position == POSITION_GEAR_UNSETED) {
        position = POSITION_GEAR_SETED;
      } else if (position == POSITION_GEAR_SETED) {
        position = POSITION_JUMP_READY;
      } else if (position == POSITION_JUMP_READY) {
        position == POSITION_GEAR_UNSETED;
      }
    } else {  // 逆転中
      if (position == POSITION_GEAR_UNSETED) {
        position = POSITION_JUMP_READY;
      } else if (position == POSITION_GEAR_SETED) {
        position = POSITION_GEAR_UNSETED;
      } else if (position == POSITION_JUMP_READY) {
        position == POSITION_GEAR_SETED;
      }
    }
    pmSensor_SetPosition(position);
  }
}
