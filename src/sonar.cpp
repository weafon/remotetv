#include "sonar.h"
// 定義聲音在空氣中的速度 (公分/微秒)
const float soundSpeed = 0.0343;

// 測量間隔時間 (毫秒)
const long measurementInterval = 200;

unsigned long lastMeasurementTime = 0;
void sonar_init()
{
	  // 設定 Trig 腳位為輸出
  pinMode(PIN_TRIG, OUTPUT);
  digitalWrite(PIN_TRIG, LOW); // 初始化為低電位

  // 設定 Echo 腳位為輸入
  pinMode(PIN_ECHO, INPUT);

}

int checkDistance() 
{
  // 確保測量間隔
  unsigned long currentTime = millis();
  if (currentTime - lastMeasurementTime >= measurementInterval) {
	lastMeasurementTime = currentTime;

	// 測量距離
	long duration = measureDistance();

	// 計算距離 (公分)
	int distance = duration * soundSpeed / 2;
	//if (distance>0) {
	//	Serial.print(distance);
	//	Serial.print(",");
	//}
	return distance;
  }
  
  return -1; // 如果還沒到測量時間，返回 -1
}

long measureDistance() {
  // 發送一個 10 微秒的觸發脈衝
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 測量 Echo 脈衝的持續時間
  long duration = pulseIn(PIN_ECHO, HIGH);

  return duration;
}