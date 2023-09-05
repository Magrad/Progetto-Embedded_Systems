#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Potentiometer
{

public:
  virtual int getCurrentValue();
  virtual int getCurrentValueMapped(int map_low, int map_high);
};

#endif