#include "DHTTemperatureSensorImpl.h"
#include "Arduino.h"

DHTTemperatureSensorImpl::DHTTemperatureSensorImpl(int pin, uint8_t type)
{
    this->pin = pin;
    this->type = type;

    dht = DHT(pin, type);
    dht.begin();
}

int DHTTemperatureSensorImpl::getTemperature()
{
    return dht.readTemperature();
}
