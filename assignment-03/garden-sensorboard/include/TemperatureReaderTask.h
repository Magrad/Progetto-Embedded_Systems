#ifndef __TEMPERATURE_READER_TASK_H__
#define __TEMPERATURE_READER_TASK_H__

#include "AnalogTemperatureSensor.h"
#include "Task.h"
#include "config.h"

class TemperatureReaderTask : public Task {

private:
    TaskHandle_t task; // Used to pass back a handle by which the created task can be referenced
    String task_name;
    float last_temp;

    TemperatureSensor* thermo;
public:
    TemperatureReaderTask(String name, int thermo_pin);
    void init();
    const char* getTaskName();
    String getTaskNameAsString();

    int readFromSensor(); // Returns a new reading from the thermometer
    int getTemperature(); // Returns a value equal to the mean on multiple readings
    void setTemperature(int value);
};

/* Inline reference of the class used as reference inside
the classless function needed as *(void)(*) required to create
a task on a Core. */
inline static TemperatureReaderTask* trt;

inline static void TemperatureInitTask(void* parameter) {
    Serial.println(trt->getTaskNameAsString() + " is running on core " + String(xPortGetCoreID()));

    const TickType_t timer = WAIT_FOR_READ / portTICK_RATE_MS;

    for(;;) {
        trt->setTemperature(trt->readFromSensor());
        delay(timer);
    }
}


#endif