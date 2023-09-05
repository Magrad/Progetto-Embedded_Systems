#ifndef __SMART_GARDEN_H__
#define __SMART_GARDEN_H__

#include "config.h"
#include "MsgServiceCode.h"
#include "ArduinoJson.h"
#include "MsgService.h"

typedef struct {
    int x;
    int y;
} Pair;

class SmartGarden {

private:
    String control;
    int day_state;
    int temperature;
    int changed_settings;
    bool manual_authority;

    bool setup_sl;
    bool setup_bt;

    Pair components[N_LEDS + 1];
    String prev_settings;
    char* components_names[N_LEDS + 1];

    MsgServiceBT* msgBT;
    MsgServiceSerial* msgSL;

    void BTInitialization();
    // void readJSONComponentsSettings_PH(char* jsonString);

public:
    SmartGarden();
    void init(int rx, int tx);
    String getCurrentState();
    int getCurrentTemperature();
    int getCurrentLight();
    int settingsChanged();
    Pair getComponentSettings(int id);
    bool successfullSetupCompleted();
    bool onManualChange();
    void setManualChangeAcknowledged();
    void setAlarmStatus();
    void clearAlarmStatus();
    void reset();

    void sendSLMsg(String content);
    void recieveSLMsg();
    void sendBTMsg(String content);
    void recieveBTMsg();

    void setCurrentDayStateTest(int value);
    int setComponentsSettings(int id, int state, int intensity);
};

#endif