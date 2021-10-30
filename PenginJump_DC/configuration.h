#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//■■■ ジャンプ設定 ■■■//
// 縄が真下に来るどれぐらい前にジャンプを始めるか[msec]
const unsigned long jump_START_TIME = 100;

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
const unsigned long peSensor_INTERVAL_MIN = 2000;
// 縄検知時の光電センサ0→光電センサ1 の間隔時間最大値[usec]
const unsigned long peSensor_INTERVAL_MAX = 1000000;

//■■■ ホールセンサ設定値 ■■■//
// 端子設定
const int hallSensor_PIN = 21;


//■■■ ESC(escControl) 設定値 ■■■//
// 端子設定
const int speedController_PIN = 3;

// 指示パルス幅の最大値[usec]
const int speedController_MAX = 2000;
// 指示パルス幅の最小値[usec]
const int speedController_MIN = 1000;

// ニュートラル時の指示パルス幅[usec]
const int speedController_NEUTRAL = (speedController_MAX + speedController_MIN)/2;


//■■■ デバッグ用ブザー設定値 ■■■//
// 端子設定
const int buzzer_PIN = 2;

#endif
