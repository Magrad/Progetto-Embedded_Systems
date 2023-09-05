#include "MakingDrinkTask.h"

/**
 * @brief Construct a new Making Drink Task:: Making Drink Task object
 *
 * @param pin
 */
MakingDrinkTask::MakingDrinkTask(int pin)
{
    this->motor_pin = pin;
}

/**
 * @brief Initialize Making Drink Task's variables
 *
 * @param period The period given from the scheduler to this task
 */
void MakingDrinkTask::init(int period)
{
    Task::init(period);

    motor = new ServoImpl(motor_pin);
    pos = 0;
    delta = FULL_SWEEP / int(T_MAKING);

    state = MAKING;
}

void MakingDrinkTask::tick()
{
    if (SmartMachine::state == MAKING_DRINK)
    {
        switch (state)
        {
        case MAKING:
        {
            // Displays the current drink that is beeing make
            SmartMachine::displayClear();
            SmartMachine::makingDrink();

            // Simulates the making of the drink
            ServoSweep();

            // Task changes state once the making of the drink has ended
            if (delta == 0)
            {
                state = COMPLETED;
            }
            break;
        }
        case COMPLETED:
        {
            // Reset variables, display that the drink is ready and changes state
            motor->off();
            pos = 0;
            delta = FULL_SWEEP / int(T_MAKING);
            SmartMachine::displayClear();
            SmartMachine::drinkReady();
            SmartMachine::state = RETRIEVING_DRINK;
            state = MAKING;
            break;
        }
        }
    }
}

void MakingDrinkTask::ServoSweep()
{
    motor->on();
    motor->setPosition(pos);
    pos += delta;

    delta = pos >= FULL_SWEEP ? 0 : delta;
}