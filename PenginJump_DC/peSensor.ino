#include "./configuration.h"

// 光電センサ数
const int peSensor_LENGTH = 2;

// 光電センサのピン
const int peSensor_Pin[] = {peSensor_PIN0, peSensor_PIN1};

// 光電センサの出力(HIGH/LOW)
volatile int peSensor_State[] = {HIGH, HIGH};

// 光電センサの出力がHI→LOWになった時刻[us]
volatile unsigned long peSensor_FallingEdge[] = {0, 0};

// 光電センサの出力がLOW→HIになった時刻[us]
volatile unsigned long peSensor_RisingEdge[] = {0, 0};

// 光電センサ0,1の取り付け角度の差[deg] 正の値
const float peSensor_DIFFDEG = peSensor_SENSOR_1_DEG - peSensor_SENSOR_0_DEG;

// 光電センサ0出力変化時の割込み関数
void peSensor_Pe0Change() {
  peSensor_Change(0);
}

// 光電センサ1出力変化時の割込み関数
void peSensor_Pe1Change() {
  peSensor_Change(1);
}

// 光電センサ出力変化時の割込み関数
void (* peSensor_ChangeEvent[])(void) = {
  peSensor_Pe0Change,
  peSensor_Pe1Change
};

// 光電センサの初期設定
void peSensor_Setup() {
  // 光電センサの安定化待ち
  delay(50);

  // ピン＆割込みの設定
  for (int i = 0; i < peSensor_LENGTH; i++) {
    pinMode(peSensor_Pin[i], INPUT_PULLUP);
    peSensor_State[i] = digitalRead(peSensor_Pin[i]);
    attachInterrupt(digitalPinToInterrupt(peSensor_Pin[i]), peSensor_ChangeEvent[i], CHANGE);
  }
}

// 光電センサ出力変化時の処理
void peSensor_Change(int no) {
  // 現在時刻[us]
  unsigned long now = micros();
  int state = digitalRead(peSensor_Pin[no]);
  peSensor_State[no] = state;
  if (state == LOW) {
    // 立下り時
    peSensor_FallingEdge[no] = now;
  } else {
    // 立上り時
    peSensor_RisingEdge[no] = now;
    if (no == 1) {
      // 縄が真下に来るの時刻の推定
      peSensor_EstimateTime();
    }
  }
}

// 縄が真下に来るの時刻の推定
void peSensor_EstimateTime() {
  unsigned long pe0_fall = peSensor_FallingEdge[0];
  unsigned long pe1_fall = peSensor_FallingEdge[1];
  unsigned long pe0_rise = peSensor_RisingEdge[0];
  unsigned long pe1_rise = peSensor_RisingEdge[1];

  // パルス幅＆パルス間隔のデバッグ表示
//  long p0w = (long)pe0_rise - (long)pe0_fall;
//  long p1w = (long)pe1_rise - (long)pe1_fall;
//  long p01w = (long)pe1_fall - (long)pe0_fall;
//  Serial.print(p0w);
//  Serial.print(", ");
//  Serial.print(p1w);
//  Serial.print(", "); 
//  Serial.println(p01w);

  if ((pe0_fall < pe0_rise) && (pe1_fall < pe1_rise) && (pe0_fall < pe1_fall)) {
    // センサ0,1 のパルス幅
    unsigned long pe0_width = pe0_rise - pe0_fall;
    unsigned long pe1_width = pe1_rise - pe1_fall;

    if ((peSensor_WIDTH_MIN < pe0_width) && (pe0_width < peSensor_WIDTH_MAX)) {
      if ((peSensor_WIDTH_MIN < pe1_width) && (pe1_width < peSensor_WIDTH_MAX)) {
        // センサ0 - センサ1 間時間
        long interval = pe1_fall - pe0_fall;
        if ((peSensor_INTERVAL_MIN < interval) && (interval < peSensor_INTERVAL_MAX)) {
          // 真下に来るまでの残り時間[usec]
          unsigned long usec = (unsigned long)(((360. - peSensor_SENSOR_1_DEG) / peSensor_DIFFDEG) * interval);
          peSensor_RisingEdge[0] = 0;
          peSensor_RisingEdge[1] = 0;
          peSensor_FallingEdge[0] = 0;
          peSensor_FallingEdge[1] = 0;
          updateBottomTime((usec + pe1_fall)/1000);
        }
      }
    }
  }
}
