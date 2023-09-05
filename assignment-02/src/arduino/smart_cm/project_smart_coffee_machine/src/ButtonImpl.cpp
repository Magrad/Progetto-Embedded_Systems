#include "ButtonImpl.h"
#include "Arduino.h"

ButtonImpl::ButtonImpl(int pin)
{
  this->pin = pin;
  state = 0;
  pinMode(pin, INPUT);
}

bool ButtonImpl::isPressed()
{
  return digitalRead(pin);
}
