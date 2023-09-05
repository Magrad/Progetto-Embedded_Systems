#ifndef __LCD__
#define __LCD__

#include "Arduino.h"

class Lcd
{

public:
    virtual void init();
    virtual void backlight();
    virtual void clear();
    virtual void print(const char *msg);
    virtual void setCursor(uint8_t x, uint8_t y);
};

#endif