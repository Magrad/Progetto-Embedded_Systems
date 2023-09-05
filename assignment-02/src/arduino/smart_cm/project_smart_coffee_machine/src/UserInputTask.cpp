#include "UserInputTask.h"

#define UP 1
#define DOWN -1

/**
 * @brief Construct a new User Input Task that initialize pin location
 * 
 * @param btn_up First Button's pin position connected to Arduino for 
 * @param btn_down Second Button's pin position connected to Arduino for 
 * @param btn_make Third Button's pin position connected to Arduino for 
 * @param pot Potentiometer's pin position connected to Arduino for 
 */
UserInputTask::UserInputTask(int btn_up, int btn_down, int btn_make, int pot)
{
    this->btn_pin[0] = btn_up;
    this->btn_pin[1] = btn_down;
    this->btn_pin[2] = btn_make;
    this->pot_pit = pot;
}

/**
 * @brief Initialize User Input Task's variables
 * 
 * @param period The period given from the scheduler to this task
 */
void UserInputTask::init(int period)
{
    Task::init(period);
    this->period = period;

    for (int i = 0; i < N_BUTTONS; i++)
    {
        buttons[i] = new ButtonImpl(btn_pin[i]);
    }

    pot = new PotentiometerImpl(pot_pit);
    state = USER;
    elapsed = 0;
}

void UserInputTask::tick()
{
    if (SmartMachine::state == USER_INTERACTION)
    {
        switch (state)
        {
        case USER:
        {
            elapsed = elapsed == 0 ? millis() : elapsed;

            // Checks if the user has pressed btn_make
            if (buttons[2]->isPressed())
            {
                SmartMachine::state = MAKING_DRINK;
                SmartMachine::sendMsg(String(STATE) + "" + String(WORKING_MSG));
            }

            if (buttons[0]->isPressed() || buttons[1]->isPressed())
            {
                // Checks if the user has pressed btn_up
                if (buttons[0]->isPressed())
                {
                    // Changes beverage displayed
                    SmartMachine::displayClear();
                    SmartMachine::changeSelected(UP);
                }
                // Checks if the user has pressed btn_down
                else if (buttons[1]->isPressed())
                {
                    // Changes beverage displayed
                    SmartMachine::displayClear();
                    SmartMachine::changeSelected(DOWN);
                }
                elapsed = millis();
                SmartMachine::lastTest = millis();
            }

            int currVal = pot->getCurrentValueMapped(0, 8);

            if (currVal != SmartMachine::sugarLevel)
            {
                // Changes sugar level to the new one and displays the updated value
                SmartMachine::sugarLevel = currVal;
                SmartMachine::displayClear();
                SmartMachine::updateDisplay();
                elapsed = millis();
            }

            // Checks if no user interaction has occured for more than T_USER_IDLE seconds 
            if (millis() - elapsed > T_USER_IDLE * SECONDS)
            {
                state = IDLE;
            }

            // Checks if no user interaction has occured for more than one second and if
            // a machine test has to occure
            if (millis() - SmartMachine::lastTest > T_CHECK * SECONDS &&
                SmartMachine::state == USER_INTERACTION &&
                elapsed > SECONDS)
            {
                SmartMachine::state = MACHINE_TEST;
                elapsed = 0;
            }
            break;
        }
        case IDLE:
        {
            // Smart Machine waits for a user to interact with it
            SmartMachine::reset();
            SmartMachine::state = WELCOME;
            elapsed = 0;
            state = USER;
            break;
        }
        }
    }
}