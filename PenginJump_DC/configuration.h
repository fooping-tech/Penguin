#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//■■■ ペンギンジャンプ設定 ■■■//
// 縄が真下に来るどれぐらい前にジャンプを始めるか[msec]
const unsigned long PenginJump_JUMP_START_TIME = 230;

// ゆっくり正転する際の出力値[%]
const int PenginJump_FORWARD_SLOW = 80;

// ゆっくり逆転する際の出力値[%]
const int PenginJump_BACKWARD_SLOW = -20;

//■■■ 光電センサ設定値 ■■■//
// 端子設定
const int peSensor_PIN0 = 11;
const int peSensor_PIN1 = 12;

// 光電センサ0,1の取り付け角度[deg] ※真下を0degとして反時計周りの角度
// 0 < peSensor_SENSOR_0_DEG < peSensor_SENSOR_1_DEG < 360 となるように設定
const float peSensor_SENSOR_0_DEG = (180. - 16.5);
const float peSensor_SENSOR_1_DEG = 180.;

// 縄検知時のパルス幅最小値[usec]
const unsigned long peSensor_WIDTH_MIN = 10;
// 縄検知時のパルス幅最大値[usec]
const unsigned long peSensor_WIDTH_MAX = 1000000L;

// 縄検知時の光電センサ0→光電センサ1 の間隔時間最小値[usec]
const long peSensor_INTERVAL_MIN = 1;
// 縄検知時の光電センサ0→光電センサ1 の間隔時間最大値[usec]
const long peSensor_INTERVAL_MAX = 10000000L;

//■■■ フォトマイクロセンサ設定値 ■■■//
// 端子設定
const int pmSensor_PIN0 = 21;
const int pmSensor_PIN1 = 20;

//■■■ DCモータ制御PWM 設定値 ■■■//
// 端子設定
const int speedController_RPWM_PIN = 3;
const int speedController_LPWM_PIN = 9;

// 正転/逆転反転モード(ギア等で向きが逆転する場合true)
// ※通常は正転で足が伸びる方向
const boolean speedController_DIR_INVERSE = false;


//■■■ 操作用ボタン設定値 ■■■//
const int button_STARTSTOP_PIN = 4;
const int button_PESENSORDISABLE_PIN = 8;
const int button_FORWARD_PIN = 14;
const int button_BACKWARD_PIN = 15;

//■■■ デバッグ用ブザー設定値 ■■■//
// 端子設定
//const int buzzer_PIN = 2;
const int buzzer_PIN = 5; // 高速カメラ用LED出力時

//■■■ ギア位置検出用ポテンショメータ 設定値 ■■■//
// 端子設定
const int gearPosition_POTENTIOMETERS_PIN = A5;

#endif
