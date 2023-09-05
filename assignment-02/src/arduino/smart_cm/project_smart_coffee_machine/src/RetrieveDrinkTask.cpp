#include "RetrieveDrinkTask.h"
#include "Arduino.h"

/**
 * @brief Construct a new Retrieve Drink Task that initialize pin location
 *
 * @param trigger Sonar's trigger pin position connected to Arduino
 * @param echo Sonar's echo pin position connected to Arduino
 * @param motor Motor's pin position connected to Arduino
 */
RetrieveDrinkTask::RetrieveDrinkTask(int trigger, int echo, int motor)
{
    this->sonar_trigger = trigger;
    this->sonar_echo = echo;
    this->motor_pin = motor;
}

/**
 * @brief Initialize Retrieve Drink Task's variables
 *
 * @param period The period given from the scheduler to this task
 */
void RetrieveDrinkTask::init(int period)
{
    Task::init(period);

    sonar = new SonarImpl(sonar_trigger, sonar_echo);
    motor = new ServoImpl(motor_pin);

    state = WATING;
    elapsed = 0;
}

void RetrieveDrinkTask::tick()
{
    if (SmartMachine::state == RETRIEVING_DRINK)
    {
        switch (state)
        {
        case WATING:
        {
            elapsed = elapsed == 0 ? millis() : elapsed;

            // If the user has retrieved the drink from the machine or if T_TIMEOUT time has passed
            // the machine goes on with his normal routine
            if (sonar->getDistance() > 0.40f || millis() - elapsed >= T_TIMEOUT * SECONDS - 100)
            {
                state = RETRIEVED;
                elapsed = 0;
            }
            break;
        }
        case RETRIEVED:
        {
            elapsed = elapsed == 0 ? millis() : elapsed;
            // Motors jump back to position 0
            motor->on();
            motor->setPosition(0);

            // Awaits 125ms for the motor to receive the impulse
            if (millis() - elapsed > ONE_EIGHTH)
            {
                motor->off();
                state = WATING;
                elapsed = 0;
                SmartMachine::state = USER_INTERACTION;
            }
            break;
        }
        }
    }
}