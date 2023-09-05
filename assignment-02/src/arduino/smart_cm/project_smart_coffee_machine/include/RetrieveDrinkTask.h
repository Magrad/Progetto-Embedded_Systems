#ifndef __RETRIEVE_DRINK_TASK__
#define __RETRIEVE_DRINK_TASK__

#include "Task.h"
#include "SonarImpl.h"
#include "ServoImpl.h"
#include "SmartMachine.h"

class RetrieveDrinkTask : public Task
{

private:
    enum
    {
        WATING,
        RETRIEVED
    } state;
    int sonar_trigger;
    int sonar_echo;
    int motor_pin;
    Sonar *sonar;
    ServoMotor *motor;

    long elapsed;

public:
    RetrieveDrinkTask(int trigger, int echo, int motor);
    void init(int period);
    void tick();
};

#endif