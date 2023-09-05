#include "BluetoothImpl.h"
#include "Arduino.h"

BluetoothImpl::BluetoothImpl(int rx_pin, int tx_pin) {
    this->rx_pin = rx_pin;
    this->tx_pin = tx_pin;

    this->channel = SoftwareSerial(this->rx_pin, this->tx_pin);
}

void BluetoothImpl::begin(int speed) {
    this->channel.begin(speed);
}

int BluetoothImpl::available() {
    return this->channel.available();
}

char BluetoothImpl::read() {
    return (char) this->channel.read();
}

void BluetoothImpl::println(char* val) {
    this->channel.println(val);
}
