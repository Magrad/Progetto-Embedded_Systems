#include "PotentiometerImpl.h"
#include "Arduino.h"

PotentiometerImpl::PotentiometerImpl(int pin)
{
    this->pin = pin;
}

int PotentiometerImpl::getCurrentValue()
{
    return analogRead(pin);
}

int PotentiometerImpl::getCurrentValueMapped(int map_low, int map_high)
{
    if (map_low > map_high)
    {
        int tmp = map_high;
        map_high = map_low;
        map_low = tmp;
    }

    return map(getCurrentValue(), 0, 1023, map_low, map_high + 1);
}
