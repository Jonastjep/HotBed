#include <Arduino.h>

#include <interface.h>
#include <pid.h>

Interface i = Interface();

void setup() {
  Serial.begin(9600);
  i.begin();
}

void loop() {
  i.run();
}

// //Thermistor
// int ThermistorPin = 0;
// int Vo;
// float R1 = 10000;
// float logR2, R2, T, Tc, Tf;
// float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
// unsigned int time = 500;
// unsigned long timer = 0;

// //PWM
// unsigned int period = 128; // 500 ms period.
// float dutyCycle; // 20% duty cycle.
// unsigned long lastChange = 0;
// bool signal = LOW;
// int pwmPin = 13;

// void setup() {
// 	Serial.begin(9600);
// 	// set up the LCD's number of columns and rows:
// 	lcd.begin(16, 2);
// 	// Clears the LCD screen
// 	lcd.clear();

// 	pinMode(pwmPin, OUTPUT);
// 	pinMode(button_blu,INPUT);
// 	pinMode(button_yel,INPUT);
// 	pinMode(button_pnk,INPUT);
// }

// void loop() {

// 	Vo = analogRead(ThermistorPin);
// 	R2 = R1 * (1023.0 / (float)Vo - 1.0);
// 	logR2 = log(R2);
// 	T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
// 	Tc = T - 273.15;

// 	if ((millis()-timer) > time){
// 	Serial.print("Temperature: ");
// 	Serial.print(Tc);
// 	Serial.println(" C");
// 	timer = millis();
// 	}

// 	// dutyCycle = map(analogRead(potPin), 0, 1023, 500, 9900) / 10000.0; //
// returns 0.05-0.99 based on the analog reading 0-1023. 	dutyCycle = map(600, 0,
// 1023, 500, 9900) / 10000.0; // returns 0.05-0.99 based on the analog reading
// 0-1023.

// 	unsigned int onTime = period * dutyCycle;
// 	unsigned int offTime = period * (1 - dutyCycle);
// 	if (signal == HIGH) {
// 	if (millis() - lastChange > onTime) {
// 		signal = LOW;
// 		lastChange += onTime;
// 	}
// 	}

// 	else {
// 	if (millis() - lastChange > offTime) {
// 		signal = HIGH;
// 		lastChange += offTime;
// 	}
// 	}
// 	digitalWrite(pwmPin, signal);

// 	//LCD AND ENCODER PART
// 	long newPosition = myEnc.read();
// 	if (newPosition != oldPosition) {
// 	oldPosition = newPosition;
// 	Serial.println(newPosition);
// 	lcd.clear();
// 	lcd.setCursor(0, 0);
// 	lcd.print("Set temp:" + String(oldPosition/4));
// 	lcd.setCursor(0, 1);
// 	lcd.print("Current temp:" + String(curTemp));
// 	}

// }
