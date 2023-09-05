#ifndef __AUTONOMOUS_STATE_TASK_H__
#define __AUTONOMOUS_STATE_TASK_H__

#include "Task.h"
#include "LedImpl.h"
#include "ServoImpl.h"
#include "SmartGarden.h"
#include "config.h"

class AutonomousStateTask : public Task {

private:
    enum { IDLE, SUNRISE, SUNDOWN, NIGHT } state; // Finite state machine

    int cur_state; // Current day state
    int last_state; // Last registered day state
    int last_light_value;
    bool motor_sleep;

    long stateTimeStamp;
    long motorTimeStamp;

    Led* led_list[N_LEDS]; // LEDs connected to the Arduino
    ServoMotor* motor; // Servo connected to the Arduino

    SmartGarden* core;

    long elapsedTime(long time);
    int getCurrentDayState(int value);

public:
    AutonomousStateTask(int leds[], int motor_pin, SmartGarden* sg);
    void tick();
    void changeState(int state); // Changes current day state to a given one

    void ledTest(int id, int state, int brightness); // Tests a given LED
    void servoTest(int speed); // Tests the servo
    void sunriseTest(); // Tests if all the LEDs will power off with a given brightness when the day begins
    void sundownTest(); // Test if all the LEDs will power on with a given brightness when it's late afternoon
    int nightTest(int speed, long time); // Test all the LEDs at full power
    void fullSimulatedTest(); // Simulates a full cycle
};

#endif