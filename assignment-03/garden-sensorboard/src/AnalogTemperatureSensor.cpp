#include "AnalogTemperatureSensor.h"

AnalogTemperatureSensor::AnalogTemperatureSensor(int pin) {
    this->pin = pin;
}

int AnalogTemperatureSensor::getTemperature(int map_from_min, int map_from_max, int max_to_max, float denominator) {
    /* Calculates the current readings from the temperature sensor and 
       and returns the result once converted into a given range */
    int value = analogRead(pin) * (FREQ / ANALOG_ESP32) / denominator;
    return map(value, map_from_min, map_from_max, 0, max_to_max);
}