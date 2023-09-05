#ifndef __ANALOG_PHOTORESISTOR_H__
#define __ANALOG_PHOTORESISTOR_H__

#include "Arduino.h"
#include "Photoresistor.h"

class AnalogPhotoresistor : public Photoresistor {

private:
    int pin; // Index of the given pin

public:
    AnalogPhotoresistor(int pin); // Assignes the index of the given pin to the photoresistor
    void init();
    int getLightValue(float map_from = ANALOG_ESP32, float map_to = ARDUINO_ANALOG);
};

#endif