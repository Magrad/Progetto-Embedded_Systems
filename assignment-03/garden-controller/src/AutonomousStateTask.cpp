#include "AutonomousStateTask.h"

AutonomousStateTask::AutonomousStateTask(int leds[], int motor_pin, SmartGarden* sg) :
    core(sg) {
    changeState(IDLE);

    /* Initialize a pointer to an array of Leds with their
    respective physical pins. */
    for(int i=0;i<N_LEDS;i++) {
        led_list[i] = new LedImpl(leds[i]);
    }
    
    /* Initialize the servo motor simulating the irrigation
    system. */
    motor = new ServoImpl(motor_pin);
    motor->off();

    motor_sleep = false;
    motorTimeStamp = millis();
    last_light_value = -1;
    last_state = -1;
}

void AutonomousStateTask::tick() {
    core->recieveSLMsg();
    core->recieveBTMsg();

    if(core->getCurrentState() == MANUAL 
        && (last_state != -1 || last_light_value != -1)) {
            last_state = -1;
            last_light_value = -1;
    }

    if(core->getCurrentState() == AUTO && core->successfullSetupCompleted()) {
        switch (cur_state) {
            case IDLE: {
                // Reads current state of day.
                int cur_light_value = core->getCurrentLight();
                int day_state = getCurrentDayState(cur_light_value);

                // Redirects to the correct state.
                if(day_state == DAY && last_state != day_state) {
                    last_state = day_state;
                    changeState(SUNRISE);
                }

                if(day_state >= AFTERNOON && last_light_value != cur_light_value) {
                    last_light_value = cur_light_value;
                    last_state = day_state;
                    changeState(SUNDOWN);
                }

                /* Checks if the motor has finished its sleep phase. */
                if(motor_sleep && elapsedTime(motorTimeStamp) > SLEEP_MIN * MINUTES) {
                    motor_sleep = false;
                }

                /* If the temperatures is over a range and the motor
                is currently asleep, the system enters an ALARM status. */
                if(motor_sleep && core->getCurrentTemperature() >= ALARM_TEMPERATURE) {
                    core->setAlarmStatus();
                }

                if(day_state == EVENING && !motor_sleep) {
                    last_state = day_state; 
                    changeState(NIGHT);
                }
            } break;
            case SUNRISE: {
                // Turn off every light.
                for(int i=0;i<N_LEDS;i++) {
                    core->setComponentsSettings(i, LED_OFF, LED_OFF);

                    // Sends to the serial line the new Led value.
                    core->sendSLMsg(String(SL_MSG) + LED_N + String(i) + "_0");
                }

                for(int i=0;i<N_LEDS;i++) {
                    led_list[i]->ledChangeState(LED_OFF);
                    led_list[i]->setBrightness();
                }
                
                // Returns to the idle state and waits for a new light reading.
                changeState(IDLE);
            } break;
            case SUNDOWN: {
                // Turn on all the lights with their respective settings.
                for(int i=0;i<N_LEDS;i++) {
                    /* The first two Leds are On/Off type Leds, so it's only
                    required to specify if they are currently on or off, their
                    brightness is always set to the max "LED_INTERVAL". */
                    if(i < ON_OFF_LEDS) {
                        core->setComponentsSettings(i, LED_ON, LED_INTERVAL);

                        // Sends to the serial line the new led value.
                        core->sendSLMsg(String(SL_MSG) + LED_N + String(i) + "_1");
                    } else {
                        /* The other two Leds are brightness Leds, so it's only
                        required to specify their current brightness, if it is
                        above 1 they will be set On, Off otherwise. */

                        int value = map(last_light_value, LED_INTERVAL, LED_ON, LED_ON, LED_INTERVAL);
                        core->setComponentsSettings(i, LED_ON, value);

                        // Sends to the serial line the new Led value.
                        core->sendSLMsg(String(SL_MSG) + LED_N + String(i) + "_" + String(value));
                    }
                }

                for(int i=0;i<N_LEDS;i++) {
                    Pair settings = core->getComponentSettings(i);

                    led_list[i]->ledChangeState(settings.x);
                    led_list[i]->setBrightness(settings.y);
                }

                // Returns to the idle state and waits for a new light reading.
                changeState(IDLE);
            } break;
            case NIGHT: {
                // Turn on the irrigation system
                int servo_speed = core->getCurrentTemperature();

                /* Checks if the timer has just entered this state for
                the first time or after a sleep. */
                if(!motor_sleep) {
                    motor_sleep = true;

                    // Starts a new time for the activity of the irrigation system.
                    motorTimeStamp = millis();
                    core->setComponentsSettings(N_LEDS, 1, servo_speed);
                    
                    // Sends to the serial line the new Servo state.
                    core->sendSLMsg(String(SL_MSG) + SERVO_MOTOR + String(N_LEDS) + "_" + String(1) + "_" + String(servo_speed));
                }

                motor->servoSweep(servo_speed);

                // Checks if the activity of the irrigation system has been completed.
                if(elapsedTime(motorTimeStamp) > ACTIVITY_SEC*SECONDS) {
                    motor->off();
                    core->setComponentsSettings(N_LEDS, 0, servo_speed);

                    // Sends to the serial line the new Servo state.
                    core->sendSLMsg(String(SL_MSG) + SERVO_MOTOR + String(N_LEDS) + "_" + String(0) + "_" + String(servo_speed));

                    // Returns to the idle state and waits for a new light reading.
                    changeState(IDLE);
                }

            } break;
        }
    } else {
        last_light_value = -1;
        last_state = -1;
    }
}

void AutonomousStateTask::changeState(int state) {
    cur_state = state;
    stateTimeStamp = millis();
}

long AutonomousStateTask::elapsedTime(long time) {
    return millis() - time;
}

int AutonomousStateTask::getCurrentDayState(int value) {
    if(value >= 5) {
        return DAY;
    } else if(value >= 2) {
        return AFTERNOON;
    } else {
        return EVENING;
    }
}

void AutonomousStateTask::ledTest(int id, int state, int brightness) {
    if(id >= N_LEDS) {
        return;
    }

    led_list[id]->ledChangeState(state);
    led_list[id]->setBrightness(brightness);
}

void AutonomousStateTask::servoTest(int speed) {
    for(int i=0;i<FULL_SWEEP;i++) {
        motor->servoSweep(speed);
    }

    motor->off();
}

void AutonomousStateTask::sunriseTest() {
    Serial.println("Sun is coming up, no need for illumination.");

    for(int i=0;i<N_LEDS;i++) {
        led_list[i]->ledChangeState(LED_OFF);
        led_list[i]->setBrightness();

        //Serial.println(String("Led_") + String(i) + "_id: " + String(led_list[i]->getLedPinTest()));
        //Serial.println(String("Led_") + String(i) + ": OFF");
    }
}

void AutonomousStateTask::sundownTest() {
    Serial.println("Sun is going down, no sufficient illumination turning on system.");

    for(int i=0;i<N_LEDS;i++) {
        Pair settings = core->getComponentSettings(i);

        led_list[i]->ledChangeState(settings.x);
        led_list[i]->setBrightness(settings.y);

        //Serial.println(String("Led_") + String(i) + "_id: " + String(led_list[i]->getLedPinTest()));
        //Serial.println(String("Led_") + String(i) + ": " + String(settings.x));
        //Serial.println(String("Led_") + String(i) + ": " + String(settings.y));
    }
}

int AutonomousStateTask::nightTest(int speed, long time) {
    motor->off();

    if(elapsedTime(stateTimeStamp) <= time) {
        motor->servoSweep(speed);
        return 0;
    }

    return -1;
}

void AutonomousStateTask::fullSimulatedTest() {
    if(elapsedTime(stateTimeStamp) % 150 != 0) {
        return;
    }

    int cur_light_value = core->getCurrentLight();
    int day_state = getCurrentDayState(cur_light_value);

    if(day_state == DAY && last_state != DAY) {
        last_state = DAY;
        sunriseTest();
    }

    if(day_state == AFTERNOON && last_state != EVENING) {
        last_state = EVENING;
        sundownTest();
    }

    if(day_state == EVENING) {
        if(nightTest(1, ACTIVITY_SEC*SECONDS) == -1) {
        }
    }
}