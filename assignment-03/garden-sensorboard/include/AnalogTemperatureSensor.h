#ifndef __ANALOG_TEMPERATURE_SENSOR_H__
#define __ANALOG_TEMPERATURE_SENSOR_H__

#include "Arduino.h"
#include "TemperatureSensor.h"

class AnalogTemperatureSensor : public TemperatureSensor {

private:
    int pin; // Index of the given pin

public:
    AnalogTemperatureSensor(int pin); // Assignes the index of the given pin to the photoresistor
    int getTemperature(int map_from_min = 0, int map_from_max = ANALOG_ESP32, int max_to_max = ANALOG, float denominator = 1);
};


#endif