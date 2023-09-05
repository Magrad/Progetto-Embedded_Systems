#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

class Bluetooth
{

public:
    virtual void begin(int speed);
    virtual int available();
    virtual char read();
    virtual void println(char* val);
};

#endif