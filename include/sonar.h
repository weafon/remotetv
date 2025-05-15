// 這些都是跟AI一起隨便寫的醜CODE 請不要當作weafon的寫作標準喔
#ifndef SONAR_H
#define SONAR_H
#include <Arduino.h>
#include <Wire.h>
const int PIN_TRIG = A2;
const int PIN_ECHO = A3;
void sonar_init() ;
int checkDistance() ;
long measureDistance() ;

#endif