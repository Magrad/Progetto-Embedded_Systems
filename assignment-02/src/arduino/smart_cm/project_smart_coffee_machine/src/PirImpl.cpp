#include "PirImpl.h"
#include "Arduino.h"

PirImpl::PirImpl(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

bool PirImpl::isDetected()
{
    return digitalRead(pin) == HIGH;
}