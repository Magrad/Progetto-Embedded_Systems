#include "WelcomeTask.h"

void WelcomeTask::init(int period)
{
    Task::init(period);

    state = FIRST_TIME;
    elapsed = 0;
}

void WelcomeTask::tick()
{
    if (SmartMachine::state == WELCOME)
    {
        switch (state)
        {
        case FIRST_TIME:
        {
            elapsed = elapsed == 0 ? millis() : elapsed;

            // Displays welcome message
            SmartMachine::displayClear();
            SmartMachine::displayPrint(WELCOME_MSG_1, 6, 0);
            SmartMachine::displayPrint(WELCOME_MSG_2, 2, 1);
            //SmartMachine::debug("Welcome");

            // After a couple of seconds changes state
            state = millis() - elapsed > WAIT * SECONDS ? NTH_TIME : state;
            break;
        }

        case NTH_TIME:
        {
            // Displays ready message
            SmartMachine::displayClear();
            SmartMachine::displayPrint(READY_MSG, 7, 0);
            // SmartMachine::debug("Ready");

            // Sends a message to the Java application changing the state of the Smart Machine
            SmartMachine::sendMsg(String(STATE) + "" + String(IDLE_MSG));
            SmartMachine::state = USER_INTERACTION;
            elapsed = 0;
            break;
        }
        }
    }
}