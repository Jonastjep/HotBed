#include <Arduino.h>
#include <pid.h>

PID::PID(double Kp = 1.5, double Ki = 0.05, double Kd = 0.1, float minTemp = 20, float maxTemp = 100){
  // temperature limits
    _minTemp = minTemp;
    _maxTemp = maxTemp;

    //PID constants
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

//period of 128 gives 500 ms period.
PWM::PWM(int pin, float period = 128, float dutyCycle = 0.5){
  _pin = pin;
  _period = period;
  _dutyCycle = dutyCycle;
  _lastChange = 0;
  _signal = LOW;
}

void PWM::begin(){
  pinMode(_pin, OUTPUT);
}

void PWM::modify_dutyCycle(float dutyCycle){
  _dutyCycle = dutyCycle;
}

void PWM::run(){
  unsigned int onTime = _period * _dutyCycle;
	unsigned int offTime = _period * (1 - _dutyCycle);
	if (_signal == HIGH) {
	if (millis() - _lastChange > onTime) {
		_signal = LOW;
		_lastChange += onTime;
	}
	}
	else {
	if (millis() - _lastChange > offTime) {
		_signal = HIGH;
		_lastChange += offTime;
	}
	}
	digitalWrite(_pin, _signal);
}

TempSens::TempSens(int pin){
  _pin = pin;

  //thermistor circuit
  float _R1 = 10000; //voltage divider resitor value
  float _c1 = 1.009249522e-03, _c2 = 2.378405444e-04, _c3 = 2.019202697e-07; //constants based on the thermistor
}

float TempSens::read_C(){
  int Vo;
  float logR2, R2, T, Tc, Tf;

  Vo = analogRead(_pin);
  R2 = _R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (_c1 + _c2*logR2 + _c3*logR2*logR2*logR2));
  Tc = T - 273.15;

  return Tc;
}

