#ifndef __PHOTORESISTOR_H__
#define __PHOTORESISTOR_H__

#include "config.h"

class Photoresistor {

public:
    virtual void init(); // Attaches to the given pin
    virtual int getLightValue(float map_from = ANALOG_ESP32, float map_to = ARDUINO_ANALOG); // Returns the current light intensity
};

#endif