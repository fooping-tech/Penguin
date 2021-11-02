#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//■■■ ペンギンジャンプ設定 ■■■//
// 縄が真下に来るどれぐらい前にジャンプを始めるか[msec]
const unsigned long PenginJump_JUMP_START_TIME = 100;

//■■■ 光電センサ設定値 ■■■//
// 端子設定
const int peSensor_PIN0 = 11;
const int peSensor_PIN1 = 12;

// 光電センサ0,1の取り付け角度[deg] ※真下を0degとして反時計周りの角度
// 0 < peSensor_SENSOR_0_DEG < peSensor_SENSOR_1_DEG < 360 となるように設定
const float peSensor_SENSOR_0_DEG = 180.;
const float peSensor_SENSOR_1_DEG = 270.;

// 縄検知時のパルス幅最小値[usec]
const unsigned long peSensor_WIDTH_MIN = 2000;
// 縄検知時のパルス幅最大値[usec]
const unsigned long peSensor_WIDTH_MAX = 100000;

// 縄検知時の光電センサ0→光電センサ1 の間隔時間最小値[usec]
const long peSensor_INTERVAL_MIN = 2000;
// 縄検知時の光電センサ0→光電センサ1 の間隔時間最大値[usec]
const long peSensor_INTERVAL_MAX = 1000000;

//■■■ フォトマイクロセンサ設定値 ■■■//
// 端子設定
const int pmSensor_PIN = 21;

enum pmSensor_POSITION {
  POSITION_UNKNOWN,       // 位置不明
  POSITION_GEAR_UNSETED,  // ジャンプ後(ギアかみ合い前)
  POSITION_GEAR_SETED,    // ジャンプ後(ギアかみ合い後)
  POSITION_JUMP_READY     // ジャンプ準備完了
};

//■■■ 加速度センサ設定 設定値 ■■■//
// ACCEL_SENSOR に設定する選択肢
#define NO_ACCEL_SENSOR   0   // 加速度センサ未使用時
#define ADAFRUIT_LSM9DS1  1   // https://www.switch-science.com/catalog/6451/
#define AE_KXR94_2050     2   // https://akizukidenshi.com/catalog/g/gM-05153/
#define AE_L3GD20H        3   // https://akizukidenshi.com/catalog/g/gK-15096/

// 加速度センサ種別
#define ACCEL_SENSOR  NO_ACCEL_SENSOR

#if ACCEL_SENSOR != NO_ACCEL_SENSOR
// センサー値の更新周期[msec]
const unsigned long accelSensor_INTERVAL = 10;

// 最新値を保持するバッファーのサイズ
const int accelSensor_BUFF_SIZE = 5;

// バッファー内の加速度最大最小の差
const float accelSensor_STABLE_ACCDIFF = 0.2;
#endif

#if ACCEL_SENSOR == AE_KXR94_2050
// 端子設定
#define accelX_PIN A3
#define accelY_PIN A4
#define accelZ_PIN A5
// 最新値を保持するリングバッファ
volatile float accelSensor_AccelX[accelSensor_BUFF_SIZE];
volatile float accelSensor_AccelY[accelSensor_BUFF_SIZE];
volatile float accelSensor_AccelZ[accelSensor_BUFF_SIZE];
volatile int accelSensor_AccelIndex = 0;
#endif

//■■■ ESC(escControl) 設定値 ■■■//
// 端子設定
const int speedController_PIN = 3;

// 指示パルス幅の最大値[usec]
const int speedController_MAX = 2000;
// 指示パルス幅の最小値[usec]
const int speedController_MIN = 1000;

// ニュートラル時の指示パルス幅[usec]
const int speedController_NEUTRAL = (speedController_MAX + speedController_MIN)/2;


//■■■ 操作用ボタン設定値 ■■■//
const int button_STARTSTOP_PIN = 4;
const int button_PESENSORDISABLE_PIN = 8;
const int button_FORWARD_PIN = 14;
const int button_BACKWARD_PIN = 15;

//■■■ デバッグ用ブザー設定値 ■■■//
// 端子設定
const int buzzer_PIN = 2;

//■■■ タイマ割り込み設定値 ■■■//
// ハードウェアタイマーの初期化
#define USING_16MHZ true
#define USING_8MHZ false
#define USING_250KHZ false

// 使用するタイマを設定
#define USE_TIMER_0 false
#define USE_TIMER_1 true
#define USE_TIMER_2 false
#define USE_TIMER_3 false

// タイマーのタイムアウト値
// ToDo: 暫定値のため要調整
const unsigned long timer_TIMEOUT = 100000;

#endif
