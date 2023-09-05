#include "Arduino.h"
#include "LedImpl.h"
#include "config.h"


LedImpl::LedImpl(int pin) {
    this->pin = pin;

    // Attaches to the given pin
    pinMode(this->pin, OUTPUT);
}

void LedImpl::ledChangeState(int value) {
    // Writes on the given pin the new LED state value
    digitalWrite(pin, value);
}

