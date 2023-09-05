#ifndef __MQTT_SERVICE_H__
#define __MQTT_SERVICE_H__

#include "WiFi.h"
#include "MQTT.h"
#include "LedImpl.h"
#include "config.h"

class MQTTService : public MQTT {

private:
    char* ssid; // given WIFI name
    char* password; // given WIFI password

    WiFiClient espClient;
    PubSubClient client;

public:
    MQTTService(); // Empty constructor
    MQTTService(int led_pin, char* ssid, char* passwrod); // Constructor
    void init();
    void recconect();

    PubSubClient* getClient();
};

/* Inline function used as classeless callback required by the 
mqtt service app. */
inline static void callback(char* topic, byte* payload, unsigned int lenght) {
    char buff[lenght + 1];
    mempcpy(buff, payload, lenght);
    buff[lenght] = NULL;

    String msg(buff);

    Serial.println("MQTT: " + msg);

    Led* state_led = new LedImpl(LED_ALARM);

    if(msg.indexOf(NOMINAL) >= 0) {
        state_led->ledChangeState(HIGH);
    }

    if(msg.indexOf(ALARM) >= 0) {
        state_led->ledChangeState(LOW);
    }

    delete state_led;
}

#endif