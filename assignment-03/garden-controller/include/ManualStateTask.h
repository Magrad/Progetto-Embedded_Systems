#ifndef __MANUAL_STATE_TASK_H__
#define __MANUAL_STATE_TASK_H__

#include "Task.h"
#include "LedImpl.h"
#include "ServoImpl.h"
#include "SmartGarden.h"
#include "config.h"

class ManualStateTask : public Task {

private:
    enum { IDLE, COMPONENTS_SETUP, ADJUSTING_LIGHTS, ADJUSTING_IRRIGATION, OPEN_IRRIGATION, CLOSE_IRRIGATION } state; // Finite state machine

    int cur_state; // Current day state
    int change_settings_id; // Last component's settings that have been changed
    bool irrigation_open;
    bool close_irrigation;

    long openTimeStamp;

    Led* led_list[N_LEDS]; // LEDs connected to the Arduino
    ServoMotor* motor; // Servo connected to the Arduino
    int motor_speed;

    SmartGarden* core;

    long elapsedTime();

public:
    ManualStateTask(int leds[], int motor_pin, SmartGarden* sg);
    void tick();
    void changeState(int state);

    void ledTest(int id, int state, int brightness); // Tests a given LED
    void servoTest(int speed); // Tests the servo
    void openIrrigation(int speed); // Tests starting  the servo
    void closeIrrigation(); // Tests stopping the servo
};

#endif