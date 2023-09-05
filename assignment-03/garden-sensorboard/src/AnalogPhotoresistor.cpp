#include "AnalogPhotoresistor.h"

AnalogPhotoresistor::AnalogPhotoresistor(int pin) {
    this->pin = pin;
}

void AnalogPhotoresistor::init() {
    pinMode(pin, INPUT);
}

int AnalogPhotoresistor::getLightValue(float map_from, float map_to) {
    // Maps and returns the given value from one range to another
    return map(analogRead(pin), 0, map_from, 0, map_to);
}