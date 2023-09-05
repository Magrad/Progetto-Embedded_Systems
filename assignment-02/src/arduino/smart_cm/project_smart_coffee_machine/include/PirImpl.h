#ifndef __PIR_IMPL__
#define __PIR_IMPL__

#include "Pir.h"

class PirImpl : public Pir
{

private:
    int pin;

public:
    PirImpl(int pin);
    bool isDetected();
};

#endif