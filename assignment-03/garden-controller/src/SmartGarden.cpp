#include "SmartGarden.h"

// Resets global variables
SmartGarden::SmartGarden() {
    reset();
}

// Initializes the serial line and bluethooth components
void SmartGarden::init(int rx, int tx) {
    msgSL = &MsgSerial;
    msgBT = new MsgServiceBT(rx, tx);

    msgSL->init();
    msgBT->init();

    //while(!Serial) {};

    sendSLMsg(String(SL_MSG) + SETUP);

    recieveSLMsg();
}

/* 
Returns the current authority of the system, can be:
NONE, at the start of the execution the system enter
the none state in which it remains till the setup has
not been completed.
AUTO, the system updates itself automatically.
MANUAL, the system is controlled by an human operator.
ALARM, the system has encountered an exeption during the
execution, for example if the temperature reaches a certain
value and the irrigation system is currently offline.
*/
String SmartGarden::getCurrentState() {
    return control;
}

/*
Returns the current temperature recieved from the
sensorboard.
*/

int SmartGarden::getCurrentTemperature() {
    return temperature;
}

/*
Returns the current light value recieved from the
sensorboard.
*/
int SmartGarden::getCurrentLight() {
    return day_state;
}

/*
Returns the id of the last component that had
it settings changed.
*/
int SmartGarden::settingsChanged() {
    int out = changed_settings;
    changed_settings = -1;
    return out;
}

// Returns the current component settings.
Pair SmartGarden::getComponentSettings(int id) {
    return components[id];
}

bool SmartGarden::successfullSetupCompleted() {
    return setup_sl && temperature != -1 && day_state != -1;
}

bool SmartGarden::onManualChange() {
    return manual_authority;
}

/*
The system enters the ALARM status, it notifies
the serialline system and the bluetooth app
*/
void SmartGarden::setAlarmStatus() {
    control = ALARM;
    sendSLMsg(String(SL_MSG) + ALARM);
    sendBTMsg(String(BT_MSG) + ALARM);

    recieveSLMsg();
}

/*
The system leaves the ALARM status, after an
operator has clicked on the alarm icon in the
phone app.
*/
void SmartGarden::clearAlarmStatus() {
    control = MANUAL;

    sendSLMsg(String(SL_MSG) + NOMINAL);
    sendBTMsg(String(BT_MSG) + NOMINAL);
}

void SmartGarden::setManualChangeAcknowledged() {
    manual_authority = false;
    changed_settings = -1;
}

/* 
Resets the global variables to their initial
states.
*/
void SmartGarden::reset() {
    changed_settings = -1;
    control = NONE;
    day_state = 7;
    temperature = -1;
    setup_sl = false;
    setup_bt = false;

    // Resets every component's settings.
    for(int i=0;i<N_COMPONENTS;i++) {
        components[i] = { 0, 0 };
        components_names[i] = "";
    }

}

/*
Sends a message to the serial line (the app
hosted on the computer that has all the garden
settings).
*/
void SmartGarden::sendSLMsg(String content) {
    msgSL->sendMsg(content);
}

/*
Function used to receive message from the
computer app.
*/
void SmartGarden::recieveSLMsg() {
    // Checks if a new message has been received
    if(msgSL->isMsgAvailable()) {
        Msg* recieved = msgSL->receiveMsg();
        String msg = recieved->getContent();
        delete recieved;

        /* Untill the setup is completed the only messages that are
        supported to be recieved from the serial line are commands to
        initialize the arduino's variables. */
        if(!setup_sl) {
            /* The serial line will send one by one the components's settings
            that are currently in use. Once that is complete i will send a 
            DONE message and within it the current state of the system. */
            if(msg.indexOf(String(SL_MSG) + DONE) == -1) {
                int count = msg.indexOf(SL_MSG) + String(SL_MSG).length();
                msg.remove(0, count);
                msg.replace("_", "");
                int id = msg[0] - '0';
                int state = msg[1] - '0';
                int intensity = msg[2] - '0';

                components[id] = {state, intensity};
            } else {
                int count = msg.indexOf(String(DONE) + ":") + String(String(DONE) + ":").length();
                msg.remove(0, count);
                control = msg == ALARM ? ALARM : AUTO;
            }

            // Checks if the setup has been successfull.
            setup_sl = control != NONE ? true : false;
            return;
        }

        /* The OK message is received once the bluetooth app has
        required controll and it has been granted from the service
        app. */
        if(msg == String(SL_MSG) + OK && setup_sl) {
            BTInitialization();

            recieveBTMsg();

            /* Lets the bluetooth app knows that the phone app initialization
            has been successfull and now the system is in MANUAL mode. */
            sendBTMsg(String(BT_MSG) + OK);
            control = MANUAL;
            manual_authority = true;
            return;
        }

        /* The CC message is received during the service app
        closure to notify the bluetooth that it can't provide anymore changes
        to the components settings. Checks if both the arduino and bluetooth
        setup have been completed. */
        if(msg.indexOf(String(SL_MSG) + CONNECTION_CLOSED) >= 0 &&
            setup_sl && setup_bt) {
                sendBTMsg(String(BT_MSG) + CONNECTION_CLOSED);
                control = AUTO;
                return;
        }
        
        /* The CMP message is received from the service line to 
        check the correctness of the new changes, applied by the
        phone app, over the garden components. */
        if (msg.indexOf(COMPONENT) >= 0) {
            /* If the service app has encoutered an error during 
            checkups it will restore the component settings to the
            last stored value. */
            if(msg.indexOf(ERR) >= 0) {
                int count = msg.indexOf(ERR) + String(ERR).length();
                prev_settings.remove(0, count);
                prev_settings.replace("_", "");
                int id = prev_settings[0] - '0';
                int state = prev_settings[1] - '0';
                int intensity = prev_settings[2] - '0';

                setComponentsSettings(id, state, intensity);
                return;
            }

            /* Creates a message to be sent to the phone app with the
            new settings of the component settings. */
            prev_settings.remove(0, String(COMPONENT).length());
            int id = prev_settings[0] - '0';
            Pair settings = getComponentSettings(id);

            String post = BT_MSG;
            post += id > ON_OFF_LEDS + BRIGHTNESS_LEDS - 1 ? SERVO_MOTOR : LED_N;

            post += id > ON_OFF_LEDS + BRIGHTNESS_LEDS - 1 ?
                        String(id) + "_" + String(settings.x) + "_" + String(settings.y) : id < ON_OFF_LEDS ?
                            String(id) + "_" + String(settings.x) : String(id) + "_" + String(settings.y);

            sendBTMsg(post);
            sendSLMsg(post);
            changed_settings = id;

            prev_settings = "";
            return;
        }

        /* The SAVE messsage is used when the phone app is connected
        and old saved settings have been found. With this change the
        operator can just open the app and already have the desired
        settings without any needed input. */
        if(msg.indexOf(SAVE) >= 0) {
            int count = msg.indexOf(SAVE) + String(SAVE).length();
            msg.remove(0, count);
            msg.replace("_", "");
            int id = msg[0] - '0';
            int state = msg[1] - '0';
            int intensity = msg[2] - '0';

            setComponentsSettings(id, state, intensity);
            
            return;
        }

        /* Retrieves the temperature readings. */
        if(msg.indexOf(TEMP) >= 0) {
            int count = msg.indexOf(TEMP) + String(TEMP).length();
            msg.remove(0, count);
            
            temperature = msg[0] - '0';
            return;
        }

        /* Retrieves the light readings. */
        if(msg.indexOf(LUM) >= 0) {
            int count = msg.indexOf(LUM) + String(LUM).length();
            msg.remove(0, count);

            day_state = msg[0] - '0';
            return;
        }
    }
}

/*
Sends a message to the bluetooth (the app
hosted on the phone that can change the current
settings of the components).
*/
void SmartGarden::sendBTMsg(String content) {
    msgBT->sendMsg(Msg(content));
}

/*
Function used to receive message from the
phone app.
*/
void SmartGarden::recieveBTMsg() {
    // Checks if a new message has been received
    if(setup_sl && msgBT->isMsgAvailable()) {
        Msg* recieved = msgBT->receiveMsg();
        String msg = recieved->getContent();
        delete recieved;

        /* The MANUAL message is received when the phone app
        requires controll. */
        if(msg.indexOf(String(BT_MSG) + MANUAL) >= 0) {
            int count = msg.indexOf(MANUAL) + String(MANUAL).length();

            msg.remove(0, count);

            // Asks authority from the service app.
            sendSLMsg(String(SL_MSG) + MANUAL);

            recieveSLMsg();
            return;
        }

        /* The AUTO message is received when the phone app has
        been closed. */
        if(msg.indexOf(String(BT_MSG) + AUTO) >= 0) {
            int count = msg.indexOf(AUTO) + String(AUTO).length();

            msg.remove(0, count);

            sendSLMsg(String(SL_MSG) + AUTO);

            recieveSLMsg();
            return;
        }

        /* The ALARM OFF message is received when the operator
        has clicked on the alarm icon in order to remove the
        ALARM state from the system and restore normal execution. */
        if(msg.indexOf(String(ALARM) + ALARM_OFF) >= 0) {
            clearAlarmStatus();
            
            sendSLMsg(String(SL_MSG) + MANUAL);

            recieveSLMsg();
            return;
        }

        /* Messages received when the operator wants to change
        a setting of one o more components. */
        if (msg.indexOf(LED_N) >= 0 || msg.indexOf(IRRIGATION) >= 0) {
            int count = msg.indexOf(LED_N) + String(LED_N).length();
            msg.remove(0, count);

            int id = isDigit(msg[0]) ? msg[0]-'0' : N_LEDS;

            Pair settings = getComponentSettings(id);
            
            prev_settings = COMPONENT + String(id) + "_" + settings.x + "_" + settings.y;
            
            String post = SL_MSG;
 
            /* If the id corrisponds to the first two leds, the only setting
            needed to be changed is the on or off state, their brigthness is
            always set to the maximum. */
            if(id <= (ON_OFF_LEDS - 1)) {
                post += (settings.x + 1) % 2 ?
                LED_N + String(id) + "_1" : LED_N + String(id) + "_0";
                setComponentsSettings(id, (settings.x + 1) % 2, settings.x ? 0 : LED_INTERVAL);
            }

            /* If the id corrisponds to the third and fourth leds, the only
            setting needed to be changed is the brightness level, based on
            that the led will be set to on or off. */
            if(id > (ON_OFF_LEDS - 1) && id <= (ON_OFF_LEDS + BRIGHTNESS_LEDS - 1)) {
                int op = msg.indexOf("+") != -1 ? 1 : -1;
                op = op == 1 && settings.y == LED_INTERVAL ?
                        0 : op == -1 && settings.y == 0 ?
                            0 : op;

                settings.y += op;

                post += LED_N + String(id) + "_" + String(settings.y);
                setComponentsSettings(id, settings.y > 0 ? 1 : 0, settings.y);
            }

            /* If the id corrisponds to the servo motor component both
            state and speed level are required for the phone app to 
            work correctly. */
            if(id > ON_OFF_LEDS + BRIGHTNESS_LEDS - 1) {
                int op = 0;

                if(msg.indexOf("+") == -1 &&
                    msg.indexOf("-") == -1) {
                    settings.x = (settings.x + 1) % 2;
                } else {
                    op = msg.indexOf("+") != -1 ? 1 : -1;

                    op = op == 1 && settings.y == SERVO_INTERVAL ?
                            0 : op == -1 && settings.y == 0 ?
                                0 : op;
                }

                settings.y += op;

                post += SERVO_MOTOR + String(id) + "_" + String(settings.x) + "_" + String(settings.y);

                setComponentsSettings(id, settings.x, settings.y);
            }

            /* Depending on the changed component it will send
            the correct message */
            sendSLMsg(post);
            return;
        }
    }

    /* If the arduino setup has not been completed and 
    messages are present on the bluetooth line it will
    inform the serial line that the setup is required
    and the bluetooth line that and error has occured. */
    if(!setup_sl && msgBT->isMsgAvailable()) {
        Msg* recieved = msgBT->receiveMsg();
        String msg = recieved->getContent();
        delete recieved;

        if(msg.indexOf(String(BT_MSG) + MANUAL) >= 0) {
            sendSLMsg(String(SL_MSG) + SETUP);
            sendBTMsg(String(BT_MSG) + ERR);
            return;
        }
    }

    if(!setup_bt && msgBT->isMsgAvailable()) {
        Msg* recieved = msgBT->receiveMsg();
        String msg = recieved->getContent();
        delete recieved;

        if(msg.indexOf(DONE) >= 0) {
            setup_bt = true;
            return;
        }

        BTInitialization();
    }
}

// Changes the current day state used during initial testing.
void SmartGarden::setCurrentDayStateTest(int value) {
    day_state = value;
}

// Changes the settings of a given component.
int SmartGarden::setComponentsSettings(int id, int state, int intensity) {
    if((id < 0 || id >= N_COMPONENTS) && state < 0 && intensity < 0) {
        return -1;
    }

    components[id].x = state;
    components[id].y = intensity;

    return 0;
}

/* Function called once the serial line has given MANUAL
controll to the phone app. This function initializes all
the component settings on the phone app. */
void SmartGarden::BTInitialization() {
    int id = 0;
    bool done = true;

    unsigned long wait_for = 125L;
    unsigned long time = millis();

    while(done) {
        if(millis() - time >= wait_for && id < N_COMPONENTS) {
            time = millis();

            String post = BT_MSG;
            post += id < N_LEDS ? LED_N : SERVO_MOTOR;
            Pair settings = getComponentSettings(id);

            post += id >= N_COMPONENTS - 1 ?
                        String(id) + "_" + String(settings.x) + "_" + String(settings.y) : id < ON_OFF_LEDS ?
                            String(id) + "_" + String(settings.x) : String(id) + "_" + String(settings.y);

            sendBTMsg(post);
            id++;
        }

        if(id >= N_COMPONENTS) {
            done = false;

            String setup = String(BT_MSG) + SETUP + ":" + DONE;

            sendBTMsg(setup);
        }
    }
 
    if(control == ALARM) {
        sendBTMsg(String(BT_MSG) + ALARM);
    }

    setup_bt = true;
}