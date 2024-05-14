#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Rotary.h>

class Button {
  private:
      bool _state;
      int _pin;

  public:
    Button(int pin);
    void begin();
    bool isReleased();
};

class Interface{
private:
  Rotary r = Rotary(11, 12); 
  Button button_yel = Button(8);
  Button button_blu = Button(9);
  Button button_pnk = Button(10);
  LiquidCrystal lcd = LiquidCrystal(7, 6, 5, 4, 3, 2);


  int columnsLCD = 16;

  int menuStatus = 0;  // This keeps track in which menu we are (0 is main menu, 1 is Set Temp)

  int menuMain_items = 3;
  char *menuMain_text[3] = {" Set Temp.", " Set Timer", " Start Cycle"};
  int menuMain_counter = 0;

  int menuTimer_items = 2;
  char *menuTimer_text[3] = {" No Timer", " Set Timer"};
  int menuTimer_counter = 0;

  int timerH = 10;
  int timerMS1 = 6;
  int timerMS2 = 10;

  int timerCounter[6] = {0,1,3,0,0,0};

  int timerPlace = 6;
  int timerPlacePos[6] = {0,1,3,4,6,7};
  int timerPlace_counter = 0;
  bool isTimer = false;

  int timeRem[3] = {0,0,0};

  int menuTemp_items = 120;
  int menuTemp_counter = 60;

  unsigned long startMillis;
  unsigned long currentMillis;
  unsigned long timerMillis;
  
public:
  int setTemp = 20;
  bool running = false;
  int currentTemp;

  Interface();
  void begin(int curTemp);
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
  void run(int curTemp);
};



