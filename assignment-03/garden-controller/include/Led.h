#ifndef __LED_H__
#define __LED_H__

class Led {

public:
    virtual void ledChangeState(int value); // Changes the current state of a LED to a given value
    virtual void setBrightness(int value = 0); // Changes the current light intensity of the LED to a given value

    virtual int getLedPinTest();
};

#endif