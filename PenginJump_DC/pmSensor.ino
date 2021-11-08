#include "./configuration.h"

unsigned long pmSensor_OnTime0 = 0;
unsigned long pmSensor_OffTime0 = 0;
unsigned long pmSensor_OnTime1 = 0;
unsigned long pmSensor_OffTime1 = 0;

unsigned long pmSensor_GetOnTime0() {
  if (pmSensor_OnTime0 == 0) {
    return 0;
  }
  return millis() - pmSensor_OnTime0;
}
unsigned long pmSensor_GetOffTime0() {
  if (pmSensor_OffTime0 == 0) {
    return 0;
  }
  return millis() - pmSensor_OffTime0;
}
unsigned long pmSensor_GetOnTime1() {
  if (pmSensor_OnTime1 == 0) {
    return 0;
  }
  return millis() - pmSensor_OnTime1;
}
unsigned long pmSensor_GetOffTime1() {
  if (pmSensor_OffTime1 == 0) {
    return 0;
  }
  return millis() - pmSensor_OffTime1;
}

// フォトマイクロセンサ初期化処理
void pmSensor_Setup() {
  pinMode(pmSensor_PIN0, INPUT);
  pinMode(pmSensor_PIN1, INPUT);

  pmSensor_ChangeEvent0();
  pmSensor_ChangeEvent1();

  attachInterrupt(digitalPinToInterrupt(pmSensor_PIN0), pmSensor_ChangeEvent0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pmSensor_PIN1), pmSensor_ChangeEvent1, CHANGE);
}


// 隙間に物がある場合はtrue
boolean pmSensor_GetState0() {
  return (digitalRead(pmSensor_PIN0) == HIGH);
}

// 隙間に物がある場合はtrue
boolean pmSensor_GetState1() {
  return (digitalRead(pmSensor_PIN1) == HIGH);
}

void pmSensor_ChangeEvent0() {
  unsigned long now = millis();
  Serial.print("  ");
  Serial.print(now);
  if (pmSensor_GetState0()) {
    Serial.println(": PM0 ON");
    pmSensor_OnTime0 = now;
    pmSensor_OffTime0 = 0;
  } else {
    Serial.println(": PM0 OFF");
    pmSensor_OnTime0 = 0;
    pmSensor_OffTime0 = now;
  }
}

void pmSensor_ChangeEvent1() {
  unsigned long now = millis();
  Serial.print("  ");
  Serial.print(millis());
  if (pmSensor_GetState1()) {
    Serial.println(": PM1 ON");
    pmSensor_OnTime1 = now;
    pmSensor_OffTime1 = 0;
  } else {
    Serial.println(": PM1 OFF");
    pmSensor_OnTime1 = 0;
    pmSensor_OffTime1 = now;
  }
}
