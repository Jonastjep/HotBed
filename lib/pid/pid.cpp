#include <Arduino.h>
#include <pid.h>

////////////////////////// CONSTANTS //////////////////////////////////
// temperature limits
float minTemp = 0.0;
float maxTemp = 100.0;

//PID constants
double Kp = 1.5;
double Ki = 0.05;
double Kd = 0.1;

//thermistor circuit
float R1 = 10000; //voltage divider resitor value
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //constants based on the thermistor
///////////////////////////////////////////////////////////////////////

TempSens::TempSens(int pin){
  _pin = pin;
}

float TempSens::read_C(){
  int Vo;
  float logR2, R2, T, Tc, Tf;

  Vo = analogRead(_pin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;

  return Tc;
}

