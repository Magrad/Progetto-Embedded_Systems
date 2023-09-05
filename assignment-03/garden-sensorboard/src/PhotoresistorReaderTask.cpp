#include "PhotoresistorReaderTask.h"

PhotoresistorReaderTask::PhotoresistorReaderTask(String name, int photores_pin) {
    task_name = name;

    photores = new AnalogPhotoresistor(photores_pin);

    last_intensity = 0;
    prt = this;
}

void PhotoresistorReaderTask::init() {
    // Starts the task on Core 0 of the ESP32.
    xTaskCreatePinnedToCore(PhotoresistorInitTask,getTaskName(),10000,NULL,1,&task,READER_TASKS);
}

const char* PhotoresistorReaderTask::getTaskName() {
    return task_name.c_str();
}

String PhotoresistorReaderTask::getTaskNameAsString() {
    return task_name;
}

int PhotoresistorReaderTask::readFromSensor() {
    return photores->getLightValue(ANALOG_ESP32, PHOTORES_MAX);
}

/* Returns the current light value retrieved from the sensor.
Since there is a delay of 5 seconds between one message and
another, to take more precise readings the values are added
together and then divided by the total number of samples that
have been taken. */
int PhotoresistorReaderTask::getLightIntensity() {
    int mean = last_intensity / (MSG_TIMEOUT / WAIT_FOR_READ);
    last_intensity = 0;
    return mean;
}

void PhotoresistorReaderTask::setLightIntensity(int value) {
    last_intensity += value;
}