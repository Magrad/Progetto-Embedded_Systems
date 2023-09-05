#include "ManualStateTask.h"

ManualStateTask::ManualStateTask(int leds[], int motor_pin, SmartGarden* sg) :
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

    change_settings_id = -1;
    irrigation_open = false;
    close_irrigation = false;
}

void ManualStateTask::tick() {
    core->recieveSLMsg();
    core->recieveBTMsg();

    if(core->getCurrentState() == MANUAL && core->successfullSetupCompleted()) {
        switch (cur_state) {
            case IDLE: {
                /* Gets the id of last component changed. */
                change_settings_id = core->settingsChanged();

                /* Enters the COMPONENTS_SETUP state only when the mobile asks
                control to initialize the leds with the current state of the app. */
                if(core->onManualChange()) {
                    core->setManualChangeAcknowledged();
                    change_settings_id = core->settingsChanged();

                    changeState(COMPONENTS_SETUP);
                    return;
                }

                if(change_settings_id == N_COMPONENTS - 1) {
                    changeState(ADJUSTING_IRRIGATION);
                    return;
                }

                if(change_settings_id >= 0) {
                    changeState(ADJUSTING_LIGHTS);
                    return;
                }

                if(irrigation_open) {
                    changeState(OPEN_IRRIGATION);
                    return;
                }
                
                if(close_irrigation) {
                    changeState(CLOSE_IRRIGATION);
                    return;
                }

            } break;
            case COMPONENTS_SETUP: {
                /* Updates every component settings with their
                respective values from the phone app. */
                for(int i=0;i<N_LEDS;i++) {
                    Pair settings = core->getComponentSettings(i);

                    led_list[i]->ledChangeState(settings.x);
                    led_list[i]->setBrightness(settings.y);
                }
                Pair settings = core->getComponentSettings(N_COMPONENTS-1);

                irrigation_open = settings.x;
                close_irrigation = settings.x;
                motor_speed = settings.y;
                
                changeState(IDLE);
            } break;
            case ADJUSTING_LIGHTS: {
                /* Changes the current settings of a given Led. */
                Pair settings = core->getComponentSettings(change_settings_id);

                led_list[change_settings_id]->ledChangeState(settings.x);
                led_list[change_settings_id]->setBrightness(settings.y);

                changeState(IDLE);
            } break;
            case ADJUSTING_IRRIGATION: {
                /* Changes the current settings of the irrigation system. */
                Pair settings = core->getComponentSettings(change_settings_id);

                irrigation_open = settings.x;
                close_irrigation = settings.x;
                motor_speed = settings.y;
                
                changeState(IDLE);
            } break;
            case OPEN_IRRIGATION: {
                motor->servoSweep(motor_speed);

                changeState(IDLE);
            } break;
            case CLOSE_IRRIGATION: {
                motor->off();
                close_irrigation = false;
                irrigation_open = false;

                changeState(IDLE);
            } break;
        }
    }
}

void ManualStateTask::changeState(int state) {
    cur_state = state;
}

void ManualStateTask::ledTest(int id, int state, int brightness) {
    if(id >= N_LEDS) {
        return;
    }

    led_list[id]->ledChangeState(state);
    led_list[id]->setBrightness(brightness);
}

void ManualStateTask::servoTest(int speed) {

    for(int i=0;i<FULL_SWEEP;i++) {
        motor->servoSweep();
    }

    motor->off();
}

void ManualStateTask::openIrrigation(int speed) {
    motor->servoSweep(speed);
}

void ManualStateTask::closeIrrigation() {
    motor->off();
}