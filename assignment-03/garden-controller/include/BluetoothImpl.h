#ifndef __BLUETOOTH_IMPL_H__
#define __BLUETOOTH_IMPL_H__

#include "SoftwareSerial.h"
#include "Wire.h"
#include "config.h"

class BluetoothImpl {

private:
    int rx_pin;
    int tx_pin;
    SoftwareSerial channel = SoftwareSerial(DEFAULT_RX_PIN, DEFAULT_TX_PIN);

public:
    BluetoothImpl(int rx_pin, int tx_pin);
    void begin(int speed);
    int available();
    char read();
    void println(char* val);
};

#endif