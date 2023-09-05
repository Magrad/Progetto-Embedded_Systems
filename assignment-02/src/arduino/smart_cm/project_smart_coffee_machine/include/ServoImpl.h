#ifndef __SERVO_IMPL__
#define __SERVO_IMPL__

#include "ServoMotor.h"
#include "Arduino.h"
#include "ServoTimer2.h"

class ServoImpl : public ServoMotor
{

private:
    int pin;
    int pos;
    int delta;
    ServoTimer2 motor;

    void on();
    void off();

public:
    ServoImpl(int pin);
    void setPosition(int angle);
    int test();
};

#endif