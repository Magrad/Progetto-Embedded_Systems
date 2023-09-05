#ifndef __IDLE_TASK__
#define __IDLE_TASK__

#include "Task.h"
#include "PirImpl.h"
#include "SmartMachine.h"

class IdleTask : public Task
{

private:
    enum
    {
        ON,
        OFF
    } state;
    int pin;
    long elapsed;
    Pir *pir;

    void sleep();

public:
    IdleTask(int pin);
    void init(int period);
    void tick();
};

#endif