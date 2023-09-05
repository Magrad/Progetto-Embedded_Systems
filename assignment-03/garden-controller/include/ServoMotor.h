#ifndef __SERVO_MOTOR_H__
#define __SERVO_MOTOR_H__

class ServoMotor {

public:
    virtual void on(); // Attaches to a given pin
    virtual void setPosition(int angle); // Sets the current rotor position to a given angle
    virtual void servoSweep(int speed = 0); // Completes a 180° sweep
    virtual void off(); // Detaches from a given pin
    virtual int test();
};

#endif