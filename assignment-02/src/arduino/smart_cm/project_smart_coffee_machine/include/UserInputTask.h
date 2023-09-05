#ifndef __USER_INPUT_TASK__
#define __USER_INPUT_TASK__

#include "Task.h"
#include "ButtonImpl.h"
#include "PotentiometerImpl.h"
#include "SmartMachine.h"

class UserInputTask : public Task
{

private:
    enum
    {
        USER,
        IDLE
    } state;
    int btn_pin[N_BUTTONS];
    int pot_pit;
    int period;
    long elapsed = 0;
    Button *buttons[N_BUTTONS];
    Potentiometer *pot;

public:
    UserInputTask(int btn_up, int btn_down, int btn_make, int pot);
    void init(int period);
    void tick();
};

#endif