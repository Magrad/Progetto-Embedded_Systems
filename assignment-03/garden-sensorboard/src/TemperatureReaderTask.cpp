#include "TemperatureReaderTask.h"

TemperatureReaderTask::TemperatureReaderTask(String name, int thermo_pin) {
    task_name = name;

    thermo = new AnalogTemperatureSensor(thermo_pin);

    last_temp = 0;
    trt = this;
}

void TemperatureReaderTask::init() {
    // Starts the task on Core 0 of the ESP32.
    xTaskCreatePinnedToCore(TemperatureInitTask,getTaskName(),10000,NULL,1,&task,READER_TASKS);
}

const char* TemperatureReaderTask::getTaskName() {
    return task_name.c_str();
}

String TemperatureReaderTask::getTaskNameAsString() {
    return task_name;
}

int TemperatureReaderTask::readFromSensor() {
    return thermo->getTemperature(LM35_MIN, LM35_MAX, THERMO_MAX, DENOMINATOR);
}

/* Returns the current temperature value retrieved from the 
sensor. Since there is a delay of 5 seconds between one message
and another, to take more precise readings the values are added
together and then divided by the total number of samples that
have been taken. */
int TemperatureReaderTask::getTemperature() {
    int mean = last_temp / (MSG_TIMEOUT / WAIT_FOR_READ);
    last_temp = 0;
    return mean;
}

void TemperatureReaderTask::setTemperature(int value) {
    last_temp += value;
}
