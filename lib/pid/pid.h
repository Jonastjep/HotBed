#include <Arduino.h>

class PID{
  public:
    PID(double Kp, double Ki, double Kd, float minVal, float maxVal);
    float compute(float setVal, float currentVal);

  private:
    // temperature limits
    float _minVal;
    float _maxVal;

    //PID constants
    double _Kp;
    double _Ki;
    double _Kd;

    double _e_prev = 0;
    double _PID_integral = 0;

    double _time;
    
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
    float dutyCycle;

    PWM(int pin, float period, float dCycle);
    void begin();
    void modify_dutyCycle(float dCycle);
    void run();
    void stop();

  private:
    int _pin;
    unsigned int _period;
    unsigned long _lastChange;
    bool _signal;
};