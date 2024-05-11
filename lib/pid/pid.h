#include <Arduino.h>

class Pid{
  public:
    Pid(int pwmPin);
    void begin();
    
};

class TempSens{
  public:
    TempSens(int pin);
    float read_C();
  private:
    int _pin;
};
