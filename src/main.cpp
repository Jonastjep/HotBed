#include <Arduino.h>

#include <interface.h>
#include <pid.h>

int pwmPin = 13;
int tempPin = 0;

float avgTemp;

float startDutyCycle = 0.99;

Interface ctrl = Interface();
TempSens thermistor = TempSens(tempPin);
PWM pwm = PWM(pwmPin, 128, startDutyCycle); //128 gives a period of 500ms

unsigned int plt_time = 500;
unsigned long plt_timer = 0;

unsigned int pid_time = 200;
unsigned long pid_timer = 0;

double kp = 0.001, ki = 0.1, kd = 0.1;
PID pid = PID(kp,ki,kd,0.1,0.9);

void setup() {
  Serial.begin(9600);

  avgTemp = thermistor.read_C();
  for(int i = 0; i < 50; i++){
    avgTemp = avgTemp*0.9 + thermistor.read_C()*0.1; // each new measurement contributes only 1/10th
    delay(5);
  }

  ctrl.begin();
  pwm.begin();

  // Starting the rolling average of temperatures
  
}

void loop() {
  ctrl.run();
  avgTemp = avgTemp*0.9 + thermistor.read_C()*0.1;
  delay(5);

  if(ctrl.running && (millis()-pid_timer) > pid_time){
    double control = pid.compute((float)ctrl.setTemp, avgTemp);
    pwm.modify_dutyCycle(0.999-control);
    pwm.run();
    pid_timer = millis();
  }else if(!ctrl.running){
    pwm.stop();
  }else{
    pwm.run();
  }
  
  if ((millis()-plt_timer) > plt_time){
    Serial.print("Temperature: ");
    Serial.print(avgTemp);
    Serial.print(",Set temp:");
    Serial.print(ctrl.setTemp);
    Serial.print(",Set temp:");
    Serial.println(pwm.dutyCycle);
    plt_timer = millis();
	}
}