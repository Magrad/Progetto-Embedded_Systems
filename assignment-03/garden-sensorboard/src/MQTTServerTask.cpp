#include "MQTTServerTask.h"

MQTTServerTask::MQTTServerTask() {

}

/* Initilizes the tasks that is runned on Core 1 that
will controll the mqtt comunication service. */
MQTTServerTask::MQTTServerTask(String task_name, MQTTService* service, TemperatureReaderTask* thermo, PhotoresistorReaderTask* photores) {
    this->task_name = task_name;
    this->service = service;

    tempTask = thermo;
    photoresTask = photores;

    // Copies the current setting to the inline class.
    mqttst = *this;

    // Initializes the MQTT service
    this->service->init();
    randomSeed(micros());
    client = this->service->getClient();
    client->setServer(MQTT_SERVER, MQTT_PORT);

    // Sets the callback to the inline method.
    client->setCallback(callback);
}

void MQTTServerTask::init() {
    xTaskCreatePinnedToCore(clientLoop,getTaskName(),10000,NULL,1,&task,MQTT_TASK);
}

const char* MQTTServerTask::getTaskName() {
    return task_name.c_str();
}

MQTTService* MQTTServerTask::getService() {
    return service;
}

PubSubClient* MQTTServerTask::getClient() {
    return client;
}

int MQTTServerTask::getLightIntensity() {
    return photoresTask->getLightIntensity();
}

int MQTTServerTask::getTemperature() {
    return tempTask->getTemperature();
}