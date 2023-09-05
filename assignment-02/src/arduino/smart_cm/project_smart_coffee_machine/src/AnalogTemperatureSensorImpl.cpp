#include "AnalogTemperatureSensorImpl.h"
#include "Arduino.h"

#define MV_VAL 1100.0d
#define ANALOG 1023.0d
#define STEP 5

AnalogTemperatureSensorImpl::AnalogTemperatureSensorImpl(int pin)
{
    this->pin = pin;
}

int AnalogTemperatureSensorImpl::getTemperature()
{
    float temp = 0.0;

    for (int i = 0; i < STEP; i++)
    {
        int v = analogRead(pin);
        temp += float(MV_VAL / ANALOG) * float(v) / float(10.0d);
    }

    return temp / STEP;
}