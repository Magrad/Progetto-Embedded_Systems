#include "ServoImpl.h"
#include "Arduino.h"

ServoImpl::ServoImpl(int pin)
{
    this->pin = pin;
    pos = 0;
    delta = 1;
}

void ServoImpl::on()
{
    motor.attach(pin);
}

void ServoImpl::setPosition(int angle)
{
    float coeff = (2250.0 - 750.0) / 180;
    angle = 750 + angle * coeff;
    motor.write(angle);
}

int ServoImpl::test()
{
    return motor.read();
}

void ServoImpl::off()
{
    motor.detach();
}
