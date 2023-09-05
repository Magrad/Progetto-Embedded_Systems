#include "AssistanceModalityTask.h"

/**
 * @brief Construct a new Assistance Modality Task that initialize pin location
 *
 * @param motor_pin Motor's pin position connected to Arduino
 * @param thermo_pin Temperature Sensor's pin position connected to Arduino
 * @param sensor_type If temperature sensor is digital, temperature sensor's type
 */
AssistanceModalityTask::AssistanceModalityTask(int motor_pin, int thermo_pin, uint8_t sensor_type)
{
    this->motor_pin = motor_pin;
    this->thermo_pin = thermo_pin;
    this->sensor_type = sensor_type;
}

/**
 * @brief Initialize Assistance Modality Task's variables
 *
 * @param period The period given from the scheduler to this task
 */
void AssistanceModalityTask::init(int period)
{
    Task::init(period);

    motor = new ServoImpl(motor_pin);
    pos = 0;
    delta = FULL_SWEEP / 5;

    temp = 0;
    nTick = 0;

    elapsed = 0;

    // Initialize an analog termomether if sensor type has not been passed as a
    // parameter
    if (sensor_type == 0)
        temp_sensor = new AnalogTemperatureSensorImpl(thermo_pin);
    else
        temp_sensor = new DHTTemperatureSensorImpl(thermo_pin, sensor_type);
}

void AssistanceModalityTask::tick()
{
    switch (SmartMachine::state)
    {
    case MACHINE_TEST:
    {
        // Display that a machine test is underway
        SmartMachine::displayClear();
        SmartMachine::displayPrint(SELF_TEST_MSG, 5, 0);

        nTick++;
        temp += temp_sensor->getTemperature();

        // Simulate some machine tests by moving the servo motor back and forth
        ServoSweep();

        elapsed = elapsed == 0 && pos < 0 ? millis() : elapsed;

        // If the servo motor test has ended checks that more than 125ms have passed
        if (pos < 0 && millis() - elapsed >= ONE_EIGHTH)
        {
            // Resets every variable for the next time they will be used
            elapsed = 0;
            pos = 0;
            delta = -delta;
            motor->off();

            // Increments number of test done and send a message to the Java application
            SmartMachine::nTests++;
            SmartMachine::sendMsg(String(N_TESTS) + "" + String(SmartMachine::nTests));
            SmartMachine::lastTest = millis();
            int currentTemp = temp / nTick;

            if (currentTemp < TEMP_MIN || currentTemp > TEMP_MAX)
            {
                // The Smart Machine enters assistance modality if the temperature is too low or high
                SmartMachine::state = MACHINE_ASSISTANCE_MODALITY;
            }
            else
            {
                // The Smart Machine goes back to the normal routine
                SmartMachine::state = WELCOME;
                delta = FULL_SWEEP / 5;
            }
        }
        break;
    }
    case MACHINE_ASSISTANCE_MODALITY:
    {
        // The Smart Machine lets the Java application know that it has entered assistance modality
        SmartMachine::displayClear();
        SmartMachine::displayPrint(ASSISTANCE_MODALITY_MSG, 0, 0);
        SmartMachine::sendMsg(String(STATE) + "" + String(ASSISTANCE_MSG));
        SmartMachine::receiveMsg();
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Test servo motor by adding some delta value to its current position
 * every time the Task ticks
 */
void AssistanceModalityTask::ServoSweep()
{
    motor->on();
    motor->setPosition(pos);
    pos += delta;

    delta = pos >= FULL_SWEEP ? -delta : delta;
}