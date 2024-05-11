#include <Arduino.h>

#include <interface.h>
#include <pid.h>

int pwmPin = 13;
int tempPin = 0;

Interface ctrl = Interface();
TempSens thermistor = TempSens(tempPin);
PWM pwm = PWM(pwmPin, 128, 0.5); //128 gives a period of 500ms

unsigned int time = 500;
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  ctrl.begin();
  pwm.begin();
}

void loop() {
  ctrl.run();
  float temp = thermistor.read_C();
  pwm.run();

  if ((millis()-timer) > time){
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    timer = millis();
	}
}
