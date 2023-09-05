#ifndef __TEMPERATURE_SENSOR_H__
#define __TEMPERATURE_SENSOR_H__

#include "config.h"

class TemperatureSensor {

public:
    virtual int getTemperature(int map_from_min = 0, int map_from_max = ANALOG_ESP32, int max_to_max = ANALOG, float denominator = 1); // Returns the current temperature
};

#endif