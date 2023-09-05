#include "Arduino.h"
#include "LedImpl.h"
#include "config.h"

LedImpl::LedImpl(int pin) {
    this->pin = pin;
    this->state = false;

    pinMode(this->pin, OUTPUT);
}

void LedImpl::ledChangeState(int value) {
    this->state = value;
}

void LedImpl::setBrightness(int value = 0) {
    this->brightness = value;

    // Maps a light intensity from one range to another
    analogWrite(this->pin, map(this->brightness, 0, LED_INTERVAL, 0, MAX_BRIGHTNESS)*this->state);
}

int LedImpl::getLedPinTest() {
    return this->pin;
}

