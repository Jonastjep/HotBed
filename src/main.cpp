#include <Arduino.h>
// #include <Encoder.h>
#include <LiquidCrystal.h>
#include <Rotary.h>

class Button {
    private:
        bool _state;
        uint8_t _pin;

    public:
        Button(uint8_t pin) : _pin(pin) {}

        void begin() {
            pinMode(_pin, INPUT_PULLUP);
            _state = digitalRead(_pin);
        }

        bool isReleased() {
            bool v = digitalRead(_pin);
            if (v != _state) {
                _state = v;
                if (_state) {
                    return true;
                }
            }
            return false;
        }
};

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

// //ENCODER AND LCD
// // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// // Encoder myEnc(11, 12);
// Rotary r = Rotary(11, 12);
// long oldPosition  = -200;
// int curTemp = 40;

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

void main_menu();
void temp_menu();
void timer_menu();
void subtimer_menu();
void cycle_menu();
void print_menu();
void selection(int *menuStatus);
void mod_counter(int *counter, int nbItems, int val);
void reset_counters();
void reset_settings();


// //BUTTONS
// int button_yel = 8;
// int button_blu = 9;
// int button_pnk = 10;
Button button_yel(8);
Button button_blu(9);
Button button_pnk(10);


Rotary r = Rotary(11, 12);  // there is no must for using interrupt pins !!

int columnsLCD = 16;

int menuStatus = 0;  // This keeps track in which menu we are (0 is main menu, 1 is Set Temp)

int menuMain_items = 3;
char *menuMain_text[] = {" Set Temp.", " Set Timer", " Start Cycle"};
int menuMain_counter = 0;

int menuTimer_items = 2;
char *menuTimer_text[] = {" No Timer", " Set Timer"};
int menuTimer_counter = 0;

int timerH = 10;
int timerMS1 = 6;
int timerMS2 = 10;

int timerCounter[] = {0,1,3,0,0,0};

int timerPlace = 6;
int timerPlacePos[] = {0,1,3,4,6,7};
int timerPlace_counter = 0;
bool isTimer = false;

int timeRem[3] = {0,0,0};

int setTemp = 50;
int menuTemp_items = 120;
int menuTemp_counter = 60;

bool running = false;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long timerMillis;

void setup() {
  button_yel.begin();
  button_blu.begin();
  button_pnk.begin();

  Serial.begin(9600);
  // Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  startMillis = millis();  // initial start time
  main_menu();
}  // End of setup

void loop() {

  unsigned char result = r.process(); //finds out in what direction the rotary encoder has been turned
  if (result) {
    int countAdd;
    
    result == DIR_CCW ? countAdd = -1 : countAdd = 1; //translates the output of r.process() into an increase of + or - 1
    
    switch (menuStatus) {
      case 0:
        mod_counter(&menuMain_counter, menuMain_items, countAdd);
        main_menu();
        break;
      case 1:
        mod_counter(&menuTemp_counter, menuTemp_items, countAdd);
        temp_menu();
        break;
      case 2:
        mod_counter(&menuTimer_counter, menuTimer_items, countAdd);
        timer_menu();
        break;
      case 3:
        cycle_menu();
        break;
      case 4:
        switch(timerPlace_counter){
          case 0:
            mod_counter(&timerCounter[0], timerH, countAdd);
            break;
          case 1:
            mod_counter(&timerCounter[1], timerH, countAdd);
            break;
          case 2:
            mod_counter(&timerCounter[2], timerMS1, countAdd);
            break;
          case 3:
            mod_counter(&timerCounter[3], timerMS2, countAdd);
            break;
          case 4:
            mod_counter(&timerCounter[4], timerMS1, countAdd);
            break;
          case 5:
            mod_counter(&timerCounter[5], timerMS2, countAdd);
            break;
        }
        subtimer_menu();
        break;

      default:
        break;
    }
  }

  // YELLOW BUTTON ACTIONS
  if (button_yel.isReleased()) {
    switch(menuStatus){
      case 1: 
        reset_counters();
        reset_settings();
        menuStatus = 0;
        print_menu();
        break;

      case 2:
        reset_counters();
        reset_settings();
        menuStatus = 0;
        print_menu();
        break;

      case 3:
        reset_counters();
        reset_settings();
        menuStatus = 0;
        print_menu();
        break;

      case 4:
        if (timerPlace_counter > 0){
          timerPlace_counter--;
          print_menu();
        }
        break;

      default:
        reset_counters();
        reset_settings();
        menuStatus = 0;
        print_menu();
        break;
    }
    
  }

  if (button_blu.isReleased()){
    switch (menuStatus){
      case 0: 
        selection(&menuStatus);
        print_menu();
        break;
      case 1:
        setTemp = menuTemp_counter;
        reset_counters();
        menuStatus = 0;
        print_menu();
        break;
      
      case 2:
        if (menuTimer_counter == 1){
          isTimer = true;
          menuStatus = 4;
          reset_counters();
          print_menu();
        }else{
          menuStatus = 0;
          reset_counters();
          menuMain_counter = 1;
          print_menu();
        }
        break;

      case 3: //This will be for a stop cycle confirmation prompt
        running = false;
        timerMillis = 0;
        menuStatus = 0;
        reset_counters();
        print_menu();
        break;

      case 4: //This is for setting the clock
        timeRem[0] = timerCounter[0]*10+timerCounter[1];
        timeRem[1] = timerCounter[2]*10+timerCounter[3];
        timeRem[2] = timerCounter[4]*10+timerCounter[5];
        menuStatus = 0;
        reset_counters();
        menuMain_counter = 1;
        print_menu();
        break;
      
      default:
        break;
    }
  }

  if (button_pnk.isReleased()) {
    switch(menuStatus){
      case 4:
        if (timerPlace_counter < timerPlace-1){
          timerPlace_counter++;
          print_menu();
        }
        break;

      default:
        break;
    }
    
  }

  if (menuStatus != 3 && running){
    running = false;
  }

  if(running){ 
    //COUNTDOWN
    currentMillis = millis();
    if(isTimer){
      if(currentMillis - timerMillis > 1000){
        timeRem[2] -= 1;
        if (timeRem[2] < 0){
          timeRem[1] -= 1;
          timeRem[2] = 59;
          if (timeRem[1] < 0){
            timeRem[0] -= 1;
            timeRem[1] = 59;
            if(timeRem[0] < 0){
              running = false;
              menuStatus = 0;
              reset_counters();
              reset_settings();
              print_menu();
            }
          }
        }
        print_menu();
        timerMillis = currentMillis;
      }
    //COUNTUP
    }else{
      if(millis() - timerMillis > 1000){
        timeRem[2] += 1;
        if (timeRem[2] > 59){
          timeRem[1] += 1;
          timeRem[2] = 0;
          if (timeRem[1] > 59){
            timeRem[0] += 1;
            timeRem[1] = 0;
          }
        }
        print_menu();
        timerMillis = currentMillis;
      }
    }
  }
}

/************FUNCTIONS**************/

void print_menu(){
  switch (menuStatus){
  case 0:
    main_menu();
    break;
  case 1:
    temp_menu();
    break;
  case 2:
    timer_menu();
    break;
  case 3:
    cycle_menu();
    break;
  case 4:
    subtimer_menu();
    break;
  }
}

void main_menu() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("   Main Menu  ");
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print("<");
  lcd.setCursor(columnsLCD - 1, 1);
  lcd.print(">");
  lcd.setCursor(1, 1);
  lcd.print(menuMain_text[menuMain_counter]);
}

void temp_menu() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Select temp.");
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print("<");
  lcd.setCursor(columnsLCD - 1, 1);
  lcd.print(">");
  lcd.setCursor(1, 1);
  lcd.print(" " + String(menuTemp_counter));
}

void timer_menu() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Select timer");
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print("<");
  lcd.setCursor(columnsLCD - 1, 1);
  lcd.print(">");
  lcd.setCursor(1, 1);
  lcd.print(menuTimer_text[menuTimer_counter]);
}

void subtimer_menu(){
  lcd.blink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Timer (hh:mm:ss)");
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print(String(timerCounter[0])+String(timerCounter[1])+":"+String(timerCounter[2])+String(timerCounter[3])+":"+String(timerCounter[4])+String(timerCounter[5]));
  lcd.setCursor(timerPlacePos[timerPlace_counter],1);
}

void cycle_menu() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Temp: " + String(setTemp) + "/" + String(setTemp));
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print("Timer: " + String(timeRem[0]) + ":" + String(timeRem[1]) + ":" + String(timeRem[2]));
}

void mod_counter(int *counter, int nbItems, int val) {
  Serial.println(*counter);
  *counter += val;
  if (*counter < 0) {
    *counter = nbItems - 1;  // roll over to last item
  }
  if (*counter > nbItems - 1) {
    *counter = 0;  // roll over to first item
  }
}

void selection(int *menuStatus) {
  switch (menuMain_counter) {
    case 0:
      *menuStatus = 1; //sets the temperature menu
      break;
    case 1:
      *menuStatus = 2; //sets the timer menu
      break;
    case 2:
      *menuStatus = 3; //sets the cycle start menu
      running = true;
      break;
    default:
      break;
  }
}

void reset_counters(){
  menuMain_counter = 0;
  menuTemp_counter = 60;
  menuTimer_counter = 0;
  timerCounter[0] = 0;
  timerCounter[1] = 1;
  timerCounter[2] = 3;
  timerCounter[3] = 0;
  timerCounter[4] = 0;
  timerCounter[5] = 0;
}

void reset_settings(){
  setTemp = 50;
  timeRem[0] = 0;
  timeRem[1] = 0;
  timeRem[2] = 0;
  isTimer = false;
}