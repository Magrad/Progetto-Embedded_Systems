#include "IdleTask.h"
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>

/**
 * @brief Construct a new Idle Task that initialize pin location
 *
 * @param pin Pir's pin position connected to Arduino
 */
IdleTask::IdleTask(int pin)
{
    this->pin = pin;
}

/**
 * @brief Initialize Idle Task's variables
 *
 * @param period The period given from the scheduler to this task
 */
void IdleTask::init(int period)
{
    Task::init(period);

    pir = new PirImpl(pin);

    state = ON;
    elapsed = 0;
}

void IdleTask::tick()
{
    switch (state)
    {
    case ON:
    {
        // A message is sent to the Java application if the Smart Machine enters idle state
        if (SmartMachine::state == MACHINE_IDLE)
        {
            SmartMachine::sendMsg(String(STATE) + " " + String(SmartMachine::state));
        }

        elapsed = elapsed == 0 ? millis() : elapsed;

        bool detected = pir->isDetected();

        elapsed = detected ? millis() : elapsed;

        // If the Smart Machine is idle and detects a movement it goes back to
        // its normal routine
        if (detected && SmartMachine::state == MACHINE_IDLE)
        {
            SmartMachine::state = WELCOME;
            // SmartMachine::debug("Detected");
        }

        if (millis() - elapsed > T_IDLE * SECONDS && !detected &&
            SmartMachine::state != MACHINE_ASSISTANCE_MODALITY)
        {
            // SmartMachine::debug("Asleep");
            SmartMachine::displayClear();
            SmartMachine::displayPrint(ASLEEP_MSG, 7, 0);
            state = OFF;

            // Smart Machine's goes to sleep after T_IDLE seconds without any movement detection
            sleep();
        }
        break;
    }
    case OFF:
    {
        state = ON;
        elapsed = 0;
        SmartMachine::state = WELCOME;
        break;
    }
    }
}

void IdleTask::sleep()
{
    delay(500);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    power_all_enable();
    // SmartMachine::debug("Awoken");
    elapsed = millis();
}