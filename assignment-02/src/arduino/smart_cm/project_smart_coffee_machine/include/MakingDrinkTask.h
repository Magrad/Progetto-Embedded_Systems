#ifndef __MAKING_DRINK_TASK__
#define __MAKING_DRINK_TASK__

#include "Task.h"
#include "ServoImpl.h"
#include "SonarImpl.h"
#include "SmartMachine.h"

class MakingDrinkTask : public Task
{

private:
    enum
    {
        MAKING,
        COMPLETED
    } state;
    int motor_pin;
    int pos;
    int delta;
    ServoMotor *motor;

    void ServoSweep();

public:
    MakingDrinkTask(int pin);
    void init(int period);
    void tick();
};

#endif