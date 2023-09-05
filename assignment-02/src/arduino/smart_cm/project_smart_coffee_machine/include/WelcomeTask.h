#ifndef __WELCOME_TASK__
#define __WELCOME_TASK__

#include "Task.h"
#include "ButtonImpl.h"
#include "SmartMachine.h"

#define WAIT 2

class WelcomeTask : public Task
{

private:
    enum
    {
        FIRST_TIME,
        NTH_TIME
    } state;
    long elapsed;

public:
    void init(int period);
    void tick();
};

#endif