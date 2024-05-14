#include <Arduino.h>

#include <LiquidCrystal.h>
#include <Rotary.h>

#include <interface.h>

////////////////////////////// INTERFACE CLASS FUNCTIONS /////////////////////////////

Interface::Interface(){

}

void Interface::begin(){
  button_yel.begin();
  button_blu.begin();
  button_pnk.begin();

  lcd.begin(16, 2);
  lcd.clear();
  startMillis = millis();  // initial start time
  main_menu();
}

void Interface::print_menu(){
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

void Interface::main_menu() {
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

void Interface::temp_menu() {
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

void Interface::timer_menu() {
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

void Interface::subtimer_menu(){
  lcd.blink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Timer (hh:mm:ss)");
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print(String(timerCounter[0])+String(timerCounter[1])+":"+String(timerCounter[2])+String(timerCounter[3])+":"+String(timerCounter[4])+String(timerCounter[5]));
  lcd.setCursor(timerPlacePos[timerPlace_counter],1);
}

void Interface::cycle_menu() {
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0, 0);  //(col, row)
  lcd.print("Temp: " + String(setTemp) + "/" + String(setTemp));
  lcd.setCursor(0, 1);  // 2nd row
  lcd.print("Timer: " + String(timeRem[0]) + ":" + String(timeRem[1]) + ":" + String(timeRem[2]));
}

void Interface::mod_counter(int *counter, int nbItems, int val) {
  // Serial.println(*counter);
  *counter += val;
  if (*counter < 0) {
    *counter = nbItems - 1;  // roll over to last item
  }
  if (*counter > nbItems - 1) {
    *counter = 0;  // roll over to first item
  }
}

void Interface::selection(int *menuStatus) {
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

void Interface::reset_counters(){
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

void Interface::reset_settings(){
  setTemp = 50;
  timeRem[0] = 0;
  timeRem[1] = 0;
  timeRem[2] = 0;
  isTimer = false;
}

void Interface::run(){

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


////////////////////////////// BUTTON CLASS FUNCTIONS ///////////////////////////

Button::Button(int pin){
  _pin = pin;
}

void Button::begin() {
  pinMode(_pin, INPUT_PULLUP);
  _state = digitalRead(_pin);
}

bool Button::isReleased() {
  bool v = digitalRead(_pin);
  if (v != _state) {
    _state = v;
    if (_state) {
      return true;
    }
  }
  return false;
}