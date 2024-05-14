#include <Arduino.h>
#include <pid.h>

PID::PID(double Kp = 1.5, double Ki = 0.05, double Kd = 0.1, float minVal = 0, float maxVal = 0.999){
  // temperature limits
    _minVal = minVal;
    _maxVal = maxVal;

    //PID constants
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;

    _time = millis();
}

float PID::compute(float setVal, float currentVal){
  float error = setVal - currentVal;

  double current_time = millis();

  double PID_p = _Kp * error;
  double PID_i = _PID_integral + _Ki * error * (current_time-_time);
  double PID_d = _Kd *  (error - _e_prev)/(current_time-_time);
  Serial.print(PID_p);
  Serial.print("  ");
  Serial.print(PID_i);
  Serial.print("  ");
  Serial.println(PID_d);
  double control = PID_p + PID_i + PID_d;

  if(control > _maxVal){
    control = _maxVal;
  }
  if(control < _minVal){
    control = _minVal;
  }

  _e_prev = error;
  _time = current_time;

  Serial.println(control);

  return control;
}

//period of 128 gives 500 ms period.
PWM::PWM(int pin, float period = 128, float dCycle = 0.5){
  _pin = pin;
  _period = period;
  dutyCycle = dCycle;
  _lastChange = 0;
  _signal = LOW;
}

void PWM::begin(){
  pinMode(_pin, OUTPUT);
}

void PWM::modify_dutyCycle(float dCycle){
  dutyCycle = dCycle;
}

void PWM::run(){
  unsigned int onTime = _period * dutyCycle;
	unsigned int offTime = _period * (1 - dutyCycle);

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

void PWM::stop(){
  _signal = LOW;
}

TempSens::TempSens(int pin){
  _pin = pin;

  //thermistor circuit
  _R1 = 10000; //voltage divider resitor value
  _c1 = 1.009249522e-03, _c2 = 2.378405444e-04, _c3 = 2.019202697e-07; //constants based on the thermistor
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

