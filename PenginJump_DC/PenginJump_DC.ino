
#include "./configuration.h"

// ジャンプ時刻が設定されていてまだ飛んでいない場合true
volatile boolean PenginJump_isJumpSeted = false;

// ジャンプ時刻[msec]
volatile unsigned long PenginJump_JumpTime = 0;

// 縄が真下にくる時刻[msec]
volatile unsigned long PenginJump_BottomTime = 0;

// ペンギンの状態を表す型
enum Pengin_STATE {
  STATE_STOP = 0,         // 停止状態
  STATE_READY = 1,        // Readyスイッチ押下後の待ち状態
  STATE_JUMPING = 2,      // ジャンプ中
  STATE_LANDING = 3,      // 着地＆安定待ち
  STATE_APPROACH = 4,     // 足を伸ばす
  STATE_JUMP_READY = 5,   // ジャンプ待ち待機(足を伸ばした状態)
  STATE_RETURN_ORIGIN = 6 // 原点復帰中
};

String Pengin_STATE_STRING[] = {
  "STOP",
  "READY",
  "JUMPING",
  "LANDING",
  "APPROACH",
  "JUMP_READY",
  "RETURN_ORIGIN"
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
  Serial.println(Pengin_STATE_STRING[state]);
  //Serial.print("Gear Position: ");
  //Serial.println(gearPosition_GetPosition());
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

#if ACCEL_SENSOR != NO_ACCEL_SENSOR
  accelSensor_Setup();
#endif

  Serial.println("setup end");

  // 入力テスト
  //  while (1) {
  //    if (button_Forward()) {
  //      Serial.println("button_Forward");
  //    }
  //    if (button_Backward()) {
  //      Serial.println("button_Backward");
  //    }
  //    if (button_StartStop()) {
  //      Serial.println("button_StartStop");
  //    }
  //    if (button_PeSensorDisable()) {
  //      Serial.println("button_PeSensorDisable");
  //    }
  //    if (pmSensor_GetState0()) {
  //      Serial.println("PmSensor 0 Detect");
  //    }
  //    if (pmSensor_GetState1()) {
  //      Serial.println("PmSensor 1 Detect");
  //    }
  //    if (digitalRead(peSensor_PIN0) == LOW) {
  //      Serial.println("PeSensor0 Detect");
  //    }
  //    if (digitalRead(peSensor_PIN1) == LOW) {
  //      Serial.println("PeSensor1 Detect");
  //    }
  //    delay(500);
  //  }

  // 加速度センサテスト
  //  while(1) {
  //    Serial.println(accelSensor_IsStable());
  //    delay(500);
  //  }

  //  // PWM出力テスト
  //  while (1) {
  //    speedController_Output(100);
  //    delay(4000);
  //    speedController_Output(80);
  //    delay(5000);
  //    speedController_Output(0);
  //    delay(2000);
  //  }

  // 角度テスト
  //  while (1) {
  //    Serial.println(gearPosition_GetPosition());
  //    delay(200);
  //  }

  PenginJump_SetState(STATE_STOP);
}

void loop() {
  unsigned long now = millis();

  Pengin_STATE state = PenginJump_GetState();

  // 停止状態
  if (state == STATE_STOP) {
    // 正転/逆転ボタン押下
    if (button_Forward()) {
      speedController_Output(PenginJump_FORWARD_SLOW);
      Serial.print("Forward: ");
      //Serial.println(gearPosition_GetPosition());
    } else if (button_Backward()) {
      speedController_Output(PenginJump_BACKWARD_SLOW);
      Serial.print("Backward: ");
      //Serial.println(gearPosition_GetPosition());
    } else {
      speedController_Stop();
      // StartStopボタン押下時は時間待ち状態へ遷移
      if (button_StartStop()) {
        gearPosition_Setup();
        PenginJump_SetState(STATE_READY);
      }
    }
  }

  // Readyスイッチ押下後の待ち状態
  if (state == STATE_READY) {
    speedController_Stop();
    // Ready状態で3秒待つ
    if (3000 < PenginJump_StateTime()) {
      PenginJump_SetState(STATE_JUMPING);
    }
  }

  // ジャンプ中の状態
  if (state == STATE_JUMPING) {
    //    float deg = gearPosition_GetPosition();
    //    // ジャンプ中の狙い角度[deg]
    //    float const jumpdeg = 210.;
    //    float diff = jumpdeg - deg;
    //
    //    if (0. < diff) {
    //      //まだ飛んでないと思われる
    //      int out = (int)(diff * (60./30.));  // 30degずれている場合は60%duty
    //      speedController_Output(out);
    //    } else {
    //      if (200 < PenginJump_StateTime()) {
    //        speedController_Output(50);
    //      } else {
    //        speedController_Stop();
    //      }
    //    }

    boolean pm0 = pmSensor_GetState0();
    boolean pm1 = pmSensor_GetState1();
    // 上端
    if (pm0 && pm1) {
      // ゆっくり上げる
      speedController_Output(60);
    }
    // ギア噛み外れ直前位置
    if (pm0 && !pm1) {
      speedController_Output(80);
    }
    // 足引き寄せ中 or 下端
    if ((!pm0 && !pm1) || (!pm0 && pm1)) {
      PenginJump_SetState(STATE_LANDING);
    }
  }

  // 着地＆安定待ち
  if (state == STATE_LANDING) {
    speedController_Stop();
    if (pmSensor_GetState0() || !pmSensor_GetState1()) {
      // 下限にいない場合は、StateTime()をリセットのため同じ状態へ遷移
      PenginJump_SetState(STATE_LANDING);
    }
    // 着地後安定 又は タイムアウト時は状態遷移
    if (accelSensor_IsStable() || 300 < PenginJump_StateTime()) {
      PenginJump_SetState(STATE_APPROACH);
    }
  }

  // 足を伸ばす状態
  if (state == STATE_APPROACH) {
    if (pmSensor_GetState0()) {
      PenginJump_SetState(STATE_JUMP_READY);
    } else {
      // ギア噛み始めは少しゆっくり
      if (PenginJump_StateTime() < 50) {
        speedController_Output(60);
      } else if (PenginJump_StateTime() < 100) {
        speedController_Output(90);
      } else {
        speedController_Output(60);
      }
    }
    //    float deg = gearPosition_GetPosition();
    //    // 狙い角度[deg]
    //    float const appdeg = 220.;
    //    float diff = appdeg - deg;
    //    if (0 < diff) {
    //      int out = (int)(diff * 60/30.);
    //      speedController_Output(out);
    //    }
    //    if (diff < 0) {
    //      PenginJump_SetState(STATE_JUMP_READY);
    //    }
  }

  // ジャンプ待ち待機(足を伸ばした状態)状態
  if (state == STATE_JUMP_READY) {
    speedController_Output(15);
    //    float deg = gearPosition_GetPosition();
    //    // 狙い角度[deg]
    //    float const readydeg = 220.;
    //    float diff = readydeg - deg;
    //    if (10 < diff) {
    //      int out = (int)(diff * 20./10.);  // 10deg で 20%duty
    //      speedController_Output(out);
    //    }

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

  //  // 原点復帰状態
  //  if (state == STATE_RETURN_ORIGIN) {
  //    // ジャンプの予定はキャンセル
  //    PenginJump_isJumpSeted = false;
  //    PenginJump_BottomTime = 0;
  //
  //    // 時間待ち(1秒)後に原点復帰を始める
  //    if (1000 < PenginJump_StateTime()) {
  //      // ギア位置がPOSITION_GEAR_UNSETEDになるまで、ゆっくり逆転させる
  //      if (pmSensor_GetPosition() == POSITION_GEAR_UNSETED) {
  //        PenginJump_SetState(STATE_STOP);
  //      } else {
  //        speedController_Output(-10);
  //      }
  //    } else {
  //      speedController_Stop();
  //    }
  //  }
  //
  //  // 動作中にStartStopボタンが押されている時は原点復帰
  //  if (state != STATE_STOP && state != STATE_READY && button_StartStop()) {
  //    PenginJump_SetState(STATE_RETURN_ORIGIN);
  //  }

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
