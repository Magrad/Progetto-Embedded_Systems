#ifndef __ASSISTANCE_MODALITY_TASK__
#define __ASSISTANCE_MODALITY_TASK__

#include "Task.h"
#include "ServoImpl.h"
#include "AnalogTemperatureSensorImpl.h"
#include "DHTTemperatureSensorImpl.h"
#include "SmartMachine.h"

class AssistanceModalityTask : public Task
{

private:
    int motor_pin;
    int thermo_pin;
    int pos;
    int delta;
    int temp;
    int nTick;
    long elapsed;
    uint8_t sensor_type;
    ServoMotor *motor;
    Thermometer *temp_sensor;

    void ServoSweep();

public:
    AssistanceModalityTask(int motor_pin, int thermo_pin, uint8_t sensor_type = 0);
    void init(int period);
    void tick();
};

#endif