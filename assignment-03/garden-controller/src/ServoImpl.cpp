#include "ServoImpl.h"
#include "Arduino.h"

ServoImpl::ServoImpl(int pin) {
    this->pin = pin;
    pos = 0;
    delta = 1;
}

void ServoImpl::on() {
    motor.attach(pin);
}

void ServoImpl::setPosition(int angle) {
    float coeff = (2250.0 - 750.0) / 180;
    angle = 750 + angle * coeff;
    motor.write(angle);
}

void ServoImpl::off() {
    motor.detach();
}

void ServoImpl::servoSweep(int speed = 0) {
    on();

    setPosition(pos);

    if(speed > SERVO_INTERVAL) {
        speed = SERVO_INTERVAL;
    }
    
    speed = map(speed, 0, SERVO_INTERVAL, 0, MAX_SPEED);

    speed = delta > 0 ? speed : -speed;

    pos += delta + speed;

    /* Once the full sweep is completed the signs
       are inverted and the motor will sweep back
       to its original position */
    if(pos >= FULL_SWEEP || pos <= 0) {
        delta = -delta;
        speed = -speed;
        pos += delta + speed;
    }
}

int ServoImpl::test() {
    return motor.read();
}
