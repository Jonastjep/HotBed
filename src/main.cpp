#include <Arduino.h>

#include <interface.h>
#include <pid.h>

int pwmPin = 13;
int tempPin = 0;

float avgTemp;

float startDutyCycle = 0.99;

Interface ctrl = Interface();
TempSens thermistor = TempSens(tempPin);
PWM pwm = PWM(pwmPin, 128, startDutyCycle); //128 gives a freq of 7.8Hz

unsigned int plt_time = 500;
unsigned long plt_timer = 0;

unsigned int pid_time = 200;
unsigned long pid_timer = 0;

double kp = 0.4, ki = 0.1, kd = 0.8;
PID pid = PID(kp,ki,kd,0.001,0.999);

void setup() {
  Serial.begin(9600);

  avgTemp = thermistor.read_C();
  for(int i = 0; i < 50; i++){
    avgTemp = avgTemp*0.95 + thermistor.read_C()*0.05; // each new measurement contributes only 1/10th
    delay(5);
  }

  ctrl.begin(avgTemp);
  pwm.begin();

  // Starting the rolling average of temperatures
  
}

void loop() {
  ctrl.run(avgTemp);
  avgTemp = avgTemp*0.95 + thermistor.read_C()*0.05;
  delay(5);

  if(ctrl.running && (millis()-pid_timer) > pid_time){
    double control = pid.compute((float)ctrl.setTemp, avgTemp);
    pwm.modify_dutyCycle(control);
    pwm.run();
    pid_timer = millis();
  }else if(!ctrl.running){
    pwm.stop();
  }else{
    pwm.run();
  }
  
  if ((millis()-plt_timer) > plt_time){
    Serial.print("Temperature:");
    Serial.print(avgTemp);
    Serial.print(", SetTemp:");
    Serial.println(ctrl.setTemp);
    plt_timer = millis();
	}
}