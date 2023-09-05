#ifndef __MQTT_H__
#define __MQTT_H__

#include "Arduino.h"
#include "PubSubClient.h"

class MQTT {

public:
    virtual void init(); // Establishes the connection to the given WIFI
    virtual void recconect(); // Tries to reconnect to the WIFI once the connection is lost
    virtual PubSubClient* getClient(); // Returns a pointer to the client

};

#endif