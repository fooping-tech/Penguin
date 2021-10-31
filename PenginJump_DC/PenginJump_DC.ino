
#include "./configuration.h"

// ジャンプ時刻が設定されていてまだ飛んでいない場合true
volatile boolean PenginJump_isJumpSeted = false;

// ジャンプ時刻[msec]
volatile unsigned long PenginJump_JumpTime = 0;

// 縄が真下にくる時刻[msec]
volatile unsigned long PenginJump_BottomTime = 0;

// ペンギンの状態を表す型
enum Pengin_STATE {
  STATE_STOP,       // 停止状態
  STATE_READY,      // Readyスイッチ押下後の待ち状態
  STATE_JUMPING,    // ジャンプ中
  STATE_LANDING,    // 着地＆安定待ち
  STATE_APPROACH,   // 足を伸ばす
  STATE_JUMP_READY  // ジャンプ待ち待機(足を伸ばした状態)
};

// ペンギンの現在状態
volatile Pengin_STATE PenginJump_State = STATE_STOP;

// 現在の状態に入った時刻[msec]
volatile unsigned long PenginJump_StateInTime = 0;

// ペンギンの状態を変更する
void PenginJump_SetState(Pengin_STATE state) {
  PenginJump_State = state;
  PenginJump_StateInTime = millis();
  Serial.print("SetState: ");
  Serial.println(state);
}

// ペンギンの現在の状態を取得する
Pengin_STATE PenginJump_GetState() {
  return PenginJump_State;
}

// 現在の状態に入ってからの経過時間[msec]
unsigned long PenginJump_StateTime() {
  return millis() - PenginJump_StateInTime;
}

void setup() {
  Serial.begin(115200);

  // 光電センサ初期化
  peSensor_Setup();

  // デバッグ用ブザー初期化
  buzzer_Setup();

  // ESC初期化
  speedController_Setup();

  // フォトマイクロセンサ初期化
  pmSensor_Setup();

  // ボタン初期化
  button_Setup();
  
#if ACCEL_SENSOR == ADAFRUIT_LSM9DS1
  //accelSensor_Setup();
#endif

  Serial.println("setup end");
}

void loop() {
  unsigned long now = millis();

  // 停止ボタンが押されている時
  if (button_Stop()) {
    PenginJump_SetState(STATE_STOP);
    
    // ジャンプの予定はキャンセル
    PenginJump_isJumpSeted = false;
    PenginJump_BottomTime = 0;
    
    // TODO 原点復帰の実装
    
  }

  Pengin_STATE state = PenginJump_GetState();

  // 停止状態
  if (state == STATE_STOP) {
    // 正転/逆転ボタン押下
    if (button_Forward()) {
      speedController_Output(10);
      Serial.println("Forward");
    } else if (button_Backward()) {
      speedController_Output(-10);
      Serial.println("Backward");
    } else {
      speedController_Stop();
      // Readyボタン押下時は時間待ち後状態遷移
      if (button_Ready()) {
        if (pmSensor_GetPosition() == POSITION_UNKNOWN) {
          pmSensor_SetPosition(POSITION_JUMP_READY);
        }
        PenginJump_SetState(STATE_READY);
      }
    }
  }

  // Readyスイッチ押下後の待ち状態
  if (state == STATE_READY) {
    // Ready状態で5秒待つ
    if (5000 < PenginJump_StateTime()) {
      PenginJump_SetState(STATE_JUMPING);
    }
  }

  // ジャンプ中の状態
  if (state == STATE_JUMPING) {
    speedController_Output(100);
    if (pmSensor_GetPosition() == POSITION_GEAR_UNSETED) {
      speedController_Output(10);
      PenginJump_SetState(STATE_LANDING);
    }
  }

  // 着地＆安定待ち状態
  if (state == STATE_LANDING) {
    if (pmSensor_GetPosition() == POSITION_GEAR_SETED) {
      Serial.println("Gear Set");
      speedController_Stop();
      // 着地後安定 又は タイムアウト時は状態遷移
      if (accelSensor_IsStable() || 300 < PenginJump_StateTime()) {
        PenginJump_SetState(STATE_APPROACH);
      }
    }
  }

  // 足を伸ばす状態
  if (state == STATE_APPROACH) {
    speedController_Output(100);
    if (pmSensor_GetPosition() == POSITION_JUMP_READY) {
      speedController_Stop();
      PenginJump_SetState(STATE_JUMP_READY);
    }
  }

  // ジャンプ待ち待機(足を伸ばした状態)状態
  if (state == STATE_JUMP_READY) {
    // 光電センサ無効化時
    if (button_PeSensorDisable() && !PenginJump_isJumpSeted) {
      PenginJump_JumpTime = now + 500;  // 500ms後にジャンプ
      buzzer_Start(); // ブザーを鳴らす
      PenginJump_isJumpSeted = true;
    }
    
    // ジャンプ開始
    if (PenginJump_isJumpSeted && (PenginJump_JumpTime <= now)) {
      PenginJump_isJumpSeted = false;
      PenginJump_SetState(STATE_JUMPING);
    }
  }

  // TODO ペンギンが倒れた場合の処理の実装

  // 縄が真下にくる時刻が設定されていて、その時刻になった場合ブザーを鳴らす
  if (PenginJump_BottomTime != 0 && PenginJump_BottomTime <= now) {
    PenginJump_BottomTime = 0;
    buzzer_Start();
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
  PenginJump_BottomTime = btime;
  PenginJump_JumpTime = btime - PenginJump_JUMP_START_TIME;
  PenginJump_isJumpSeted = true;
  Serial.println("Jump Time Seted");
}
