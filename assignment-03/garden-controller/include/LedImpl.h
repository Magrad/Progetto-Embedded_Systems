#ifndef __LED__IMPL_H__
#define __LED__IMPL_H__

#include "Led.h"

class LedImpl : public Led {

private:
    int pin; // Index of the given pin
    int state; // Current LED state (ON, OFF)
    int brightness; // Current light intensity

public:
    LedImpl(int pin); // Attaches to a given pin
    void ledChangeState(int value);
    void setBrightness(int value = 0);
    
    int getLedPinTest();
};

#endif