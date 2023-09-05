#ifndef __SERVO_IMPL_H__
#define __SERVO_IMPL_H__

#include "ServoMotor.h"
#include "Arduino.h"
#include "ServoTimer2.h"
#include "config.h"

class ServoImpl : public ServoMotor {

private:
    int pin; // Index of the given pin
    int pos; // Current rotor position
    int delta; // Current rotor direction
    int speed;
    ServoTimer2 motor;

    void on();
    void off();

public:
    ServoImpl(int pin);
    void setPosition(int angle);
    void servoSweep(int speed = 0);
    int test();
};

#endif