#include "./configuration.h"

#if ACCEL_SENSOR == AE_KXR94_2050
// ライブラリマネージャーで "megaAVR_TimerInterrupt"を検索してインストール
// https://github.com/khoih-prog/megaAVR_TimerInterrupt
// タイマーの設定後インクルードすること（設定はconfiguratin.hに記載）
#include <megaAVR_TimerInterrupt.h>
#include <megaAVR_ISR_Timer.h>
#endif

// 加速度センサ無しの場合の実装
#if ACCEL_SENSOR == NO_ACCEL_SENSOR
boolean accelSensor_IsStable() {
  return true;
}
#endif

// 加速度センサありの場合の共通処理
#if ACCEL_SENSOR != NO_ACCEL_SENSOR
// 最新値を保持するリングバッファ
volatile float accelSensor_AccelX[accelSensor_BUFF_SIZE];
volatile float accelSensor_AccelY[accelSensor_BUFF_SIZE];
volatile float accelSensor_AccelZ[accelSensor_BUFF_SIZE];
volatile int accelSensor_AccelIndex = 0;

boolean accelSensor_IsStable() {
  // リングバッファ内の最大最小値を求める
  float acc_max = accelSensor_AccelZ[0];
  float acc_min = acc_max;
  for (int i = 1; i < accelSensor_BUFF_SIZE; i++) {
    float acc = accelSensor_AccelZ[i];
    if (acc < acc_min) {
      acc_min = acc;
    }
    if (acc_max < acc) {
      acc_max = acc;
    }
  }
  boolean stable = acc_max - acc_min < accelSensor_STABLE_ACCDIFF;
  return stable;
}
#endif

// 加速度センサ AE_KXR94_2050 使用時の処理
#if ACCEL_SENSOR == AE_KXR94_2050

// 加速度センサ初期化
void accelSensor_Setup() {
  ITimer1.init();
  if (ITimer1.attachInterruptInterval(accelSensor_INTERVAL, accelSensor_ReadAll)) {
    Serial.println("Starting ITimer1 OK");
  } else {
    Serial.println("Can't set ITimer1. Select another freq. or timer");
  }
}

// 加速度データ読み取り処理
// ※タイマー割込みから呼ばれるため(delay()の使用禁止)
void accelSensor_ReadAll() {
  int accxad = analogRead(accelSensor_ACCELX_PIN);
  int accyad = analogRead(accelSensor_ACCELY_PIN);
  int acczad = analogRead(accelSensor_ACCELZ_PIN);

  int index = accelSensor_AccelIndex;
  accelSensor_AccelX[index] = ((float)accxad * (5. / 1023.) - accelSensor_VOLT_OFFSET) * accelSensor_SENSITIVITY;
  accelSensor_AccelY[index] = ((float)accyad * (5. / 1023.) - accelSensor_VOLT_OFFSET) * accelSensor_SENSITIVITY;
  accelSensor_AccelZ[index] = ((float)acczad * (5. / 1023.) - accelSensor_VOLT_OFFSET) * accelSensor_SENSITIVITY;
  index++;
  if (accelSensor_BUFF_SIZE <= index) {
    index = 0;
  }
  accelSensor_AccelIndex = index;
}

#endif
