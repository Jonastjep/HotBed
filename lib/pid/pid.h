#include <Arduino.h>

class PID{
  public:
    PID(double Kp, double Ki, double Kd, float minTemp, float maxTemp);
    void begin();

  private:
    // temperature limits
    float _minTemp;
    float _maxTemp;

    //PID constants
    double _Kp;
    double _Ki;
    double _Kd;
    
};

class TempSens{
  public:
    TempSens(int pin);
    float read_C();

  private:
    int _pin;

    //thermistor circuit
    float _R1; //voltage divider resitor value
    float _c1, _c2, _c3; //constants based on the thermistor
};

class PWM{
  public:
    PWM(int pin, float period, float dutyCycle);
    void begin();
    void modify_dutyCycle(float dutyCycle);
    void run();

  private:
    int _pin;
    float _dutyCycle;
    unsigned int _period;
    unsigned long _lastChange;
    bool _signal;
};