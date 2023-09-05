#ifndef __LED__IMPL_H__
#define __LED__IMPL_H__

#include "Led.h"

class LedImpl : public Led {

private:
    int pin; // Index of the given pin

public:
    LedImpl(int pin); // Assignes the index of the given pin to the photoresistor
    void ledChangeState(int value);
};

#endif