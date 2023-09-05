#ifndef __ANALOG_TEMPERATURE_SENSOR_IMPL__
#define __ANALOG_TEMPERATURE_SENSOR_IMPL__

#include "Thermometer.h"

class AnalogTemperatureSensorImpl : public Thermometer
{

private:
    int pin;

public:
    AnalogTemperatureSensorImpl(int pin);
    int getTemperature();
};

#endif