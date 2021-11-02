#include "./configuration.h"
#include <megaAVR_TimerInterrupt.h>
#include <megaAVR_ISR_Timer.h>

boolean accelSensor_IsStable() {
  // TODO スタブのため要実装
  return false;
}

#if ACCEL_SENSOR == ADAFRUIT_LSM9DS1
// ライブラリマネージャーで　"Adafruit LSM9DS1" を検索してライブラリをインストール
// https://learn.adafruit.com/adafruit-lsm9ds1-accelerometer-plus-gyro-plus-magnetometer-9-dof-breakout/arduino-code

#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

void accelSensor_Setup() {
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
}

float accelSensor_ReadZ() {
  lsm.readAccel();
  sensors_event_t a;
  lsm.getEvent(&a, NULL, NULL, NULL);

  //Serial.println(a.acceleration.z);
  return a.acceleration.z;
}

// 安定するまで待つ（タイムアウト[msec]付き）
boolean accelSensor_WaitStable(unsigned long timeout) {
  unsigned long timelimit = millis() + timeout;
  int index = 0;
  const int listlen = accelSensor_BUFF_SIZE;
  float list[listlen];
  boolean listfull = false;
  while (millis() < timeout) {
    list[index++] = accelSensor_ReadZ();
    if (listlen <= index) {
      listfull = true;
      index = 0;
    }
    if (listfull) {
      // list内の最大最小値を求める
      float acc_max = list[0];
      float acc_min = list[0];
      for (int i=1; i<listlen; i++) {
        if (list[i] < acc_min) {
          acc_min = list[i];
        }
        if (acc_max < list[i]) {
          acc_max = list[i];
        }
      }
      if (acc_max - acc_min < accelSensor_STABLE_ACCDIFF) {
        Serial.println(1);
        //return true;
      } else {
        Serial.println(0);
      }
      delay(accelSensor_INTERVAL);
    }
  }
  return false;
}

#endif

#if ACCEL_SENSOR == AE_KXR94_2050

void accelSensor_Setup() {
  ITimer1.init();
  if (ITimer1.attachInterruptInterval(accelSensor_INTERVAL, accelSensor_WaitStable, timer_TIMEOUT, 0)) {
    Serial.println("Starting ITimer1 OK, millis() = " + String(millis());
  }eles{
    Serial.println("Can't set ITimer1. Select another freq. or timer");
  }
}

float accelSensor_ReadZ() {
  float accelZ = analogRead(accelZ_PIN);
  //Serial.println(z);
  return accelZ;
}

// 安定するまで待つ（タイムアウト[msec]付き）
boolean accelSensor_WaitStable(unsigned long timeout) {
  unsigned long timelimit = millis() + timeout;
  int index = 0;
  const int listlen = accelSensor_BUFF_SIZE;
  float list[listlen];
  boolean listfull = false;
  while (millis() < timeout) {
    list[index++] = accelSensor_ReadZ();
    if (listlen <= index) {
      listfull = true;
      index = 0;
    }
    if (listfull) {
      // list内の最大最小値を求める
      float acc_max = list[0];
      float acc_min = list[0];
      for (int i=1; i<listlen; i++) {
        if (list[i] < acc_min) {
          acc_min = list[i];
        }
        if (acc_max < list[i]) {
          acc_max = list[i];
        }
      }
      if (acc_max - acc_min < accelSensor_STABLE_ACCDIFF) {
        Serial.println(1);
        //return true;
      } else {
        Serial.println(0);
      }
    }
  }
  return false;
}

#endif
