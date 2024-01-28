#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//■■■ ペンギンジャンプ設定 ■■■//
// 縄が真下に来るどれぐらい前にジャンプを始めるか[msec]
const unsigned long PenginJump_JUMP_START_TIME = 230;//230ms
//光センサ検出後 初回ジャンプ待ち時間[us]
const unsigned long peSensor_CALC_WAITTIME =410000;
//光センサ検出後 2回以降ジャンプ待ち時間[us]
const unsigned long peSensor_CALC_WAITTIME2 =350000;

// ゆっくり正転する際の出力値[%]
const int PenginJump_FORWARD_SLOW = 100;//80

// ゆっくり逆転する際の出力値[%]
const int PenginJump_BACKWARD_SLOW = -100;//-20

// ジャンプ上端まで登る際の出力値[%] （上げすぎるとギア外れて落下中にぶつかるリスク。下げすぎると登れない）
const int PenginJump_JUMPING_RISE = 85;

// ギア噛み外れ直前上端まで登る際の出力値[%]
const int PenginJump_JUMPING_RISE_SLOW = 80;

// 着地後安定待ち時間[ms]
const int PenginJump_LANDING_TIME = 150;

// 動き出しギア噛み込みまで[ms]とDUTY[%]
const int PenginJump_APPROACH_TIME1 = 20;
const int PenginJump_APPROACH_DUTY1 = 60;

// 動き出しギア噛み込み〜上端到達間際[ms]とDUTY[%]
const int PenginJump_APPROACH_TIME2 = 150;
const int PenginJump_APPROACH_DUTY2 = 100;
//上端到達間際DUTY[%]
const int PenginJump_APPROACH_DUTY3 = 60;

//上端到達ブレーキDUTY[%]
const int PenginJump_APPROACH_BRAKE_DUTY = -20;
//上端到達ブレーキ時間[ms]
const int PenginJump_APPROACH_BRAKE_TIME = 10;

//ジャンプ待ち落ちるギリギリ保持DUTY[%]
const int PenginJump_JUMP_READY_DUTY1 =0;
//ジャンプ待ち保持DUTY[%]
const int PenginJump_JUMP_READY_DUTY2 =10;
//ジャンプ待ち少し下がってPhotoセンサ両off時の上りDUTY[%]
const int PenginJump_JUMP_READY_DUTY3 =50;

//Startボタン押した後の動遷移待ち時間[ms]
const int PenginJump_READY_TIME = 3000;


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
const unsigned long peSensor_WIDTH_MAX = 200000;

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
const int speedController_RPWM_PIN = 9;
const int speedController_LPWM_PIN = 3;

// 正転/逆転反転モード(ギア等で向きが逆転する場合true)
// ※通常は正転で足が伸びる方向
const boolean speedController_DIR_INVERSE = false;


//■■■ 操作用ボタン設定値 ■■■//
const int button_STARTSTOP_PIN = 5;//4->5
const int button_PESENSORDISABLE_PIN = 8;
const int button_FORWARD_PIN = 14;
const int button_BACKWARD_PIN = 15;

//■■■ デバッグ用ブザー設定値 ■■■//
// 端子設定
//const int buzzer_PIN = 2;
const int buzzer_PIN = 10; // 高速カメラ用LED出力時

//■■■ ギア位置検出用ポテンショメータ 設定値 ■■■//
// 端子設定
const int gearPosition_POTENTIOMETERS_PIN = A5;

#endif
