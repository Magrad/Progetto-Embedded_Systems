#ifndef __POTENTIOMETER_IMPL__
#define __POTENTIOMETER_IMPL__

#include "Potentiometer.h"

class PotentiometerImpl : public Potentiometer
{

private:
  int pin;

public:
  PotentiometerImpl(int pin);
  int getCurrentValue();
  int getCurrentValueMapped(int map_low, int map_high);
};

#endif
