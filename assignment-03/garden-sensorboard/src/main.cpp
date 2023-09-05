#include <Arduino.h>
#include "TemperatureReaderTask.h"
#include "PhotoresistorReaderTask.h"
#include "MQTTServerTask.h"

Task* tempTask;
Task* photoresTask;
MQTT* mqtt_client;

MQTTServerTask* mqttTask;


void setup() {
    Serial.begin(115200);

    /* Creates the component reader tasks on core 0 and
    the mqtt comunication line on core 1. */
    tempTask = new TemperatureReaderTask(THERMO_TASK, THERMO_PIN);
    photoresTask = new PhotoresistorReaderTask(PHOTORES_TASK, PHOTORES_PIN);   
    mqtt_client = new MQTTService(LED_ALARM, SSID_WIFI, PASSWORD_WIFI);
    mqttTask = new MQTTServerTask(MQTTSERVER_TASK, (MQTTService*)mqtt_client, (TemperatureReaderTask*)tempTask, (PhotoresistorReaderTask*)photoresTask);

    tempTask->init();
    photoresTask->init();
    mqttTask->init();
}

void loop() {
}
