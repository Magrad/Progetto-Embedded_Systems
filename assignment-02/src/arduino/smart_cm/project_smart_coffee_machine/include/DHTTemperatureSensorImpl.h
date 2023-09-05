#ifndef __DHT_TEMPERATURE_SENSOR_IMPL__
#define __DHT_TEMPERATURE_SENSOR_IMPL__

#include "Thermometer.h"
#include "DHT_modified.h"

class DHTTemperatureSensorImpl : public Thermometer
{

private:
    int pin;
    uint8_t type;
    DHT dht;

public:
    DHTTemperatureSensorImpl(int pin, uint8_t type);
    int getTemperature();
};

#endif
