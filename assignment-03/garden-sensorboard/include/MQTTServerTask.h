#ifndef __MQTT_SERVER_TASK_H__
#define __MQTT_SERVER_TASK_H__

#include "MQTTService.h"
#include "PhotoresistorReaderTask.h"
#include "TemperatureReaderTask.h"

class MQTTServerTask {

private:
    TaskHandle_t task; // Used to pass back a handle by which the created task can be referenced
    String task_name;

    MQTTService* service;
    PubSubClient* client; // Sotres a pointer to the PubSub client for comunication purposes

    // Stores pointers to the readers tasks
    TemperatureReaderTask* tempTask;
    PhotoresistorReaderTask* photoresTask;


public:
    MQTTServerTask(); // Empty constructor
    MQTTServerTask(String task_name, MQTTService* service, TemperatureReaderTask* thermo, PhotoresistorReaderTask* photores);
    void init();
    const char* getTaskName();

    MQTTService* getService(); // Returns a pointer to the MQTT service
    PubSubClient* getClient(); // Returns a pointer to the PubSub client

    int getLightIntensity(); // Returns the light readings
    int getTemperature(); // Returns the temperature readings

    void loopTester();
};

/* Inline reference of the class used as reference inside
the classless function needed as *(void)(*) required to create
a task on a Core. */
inline static MQTTServerTask mqttst;

inline static void clientLoop(void* parameter) {
    // Prints out informations about the current activity that is going to be runned on this Core.
    Serial.println(String(mqttst.getTaskName()) + " is running on core " + String(xPortGetCoreID()));

    /* Don't really understands how this works, but after many tries it seems that this 
    solution is the only one working correctly. */
    PubSubClient* client = mqttst.getClient();

    char msg[MSG_BUFFER_SIZE];
    long timeout = millis();
    const TickType_t timer = WAIT_TIMER / portTICK_RATE_MS;

    for(;;) {

        if(!client->connected()) {
            mqttst.getService()->recconect();
        }
        client->loop();

        if((long)millis() - timeout > MSG_TIMEOUT) {
            timeout = millis();

            /* Publishes the current temperature and light readings. */

            snprintf(msg, MSG_BUFFER_SIZE, String(String("Temperature: ") + String(mqttst.getTemperature())).c_str());

            Serial.println(String("Publishing message: ") + msg);

            client->publish("smart-garden-2122", msg);

            snprintf(msg, MSG_BUFFER_SIZE, String(String("Luminosity: ") + String(mqttst.getLightIntensity())).c_str());

            Serial.println(String("Publishing message: ") + msg);

            client->publish("smart-garden-2122", msg);
        }

        delay(timer);
    }
}

#endif