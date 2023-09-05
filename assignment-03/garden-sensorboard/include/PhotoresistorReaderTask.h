#ifndef __PHOTORESISTOR_READER_TASK_H__
#define __PHOTORESISTOR_READER_TASK_H__

#include "AnalogPhotoresistor.h"
#include "Task.h"
#include "config.h"

class PhotoresistorReaderTask : public Task {

private:
    TaskHandle_t task; // Used to pass back a handle by which the created task can be referenced
    String task_name;
    int last_intensity;

    Photoresistor* photores;

    long elapsedTime();

public:
    PhotoresistorReaderTask(String name, int photores_pin);
    void init();
    const char* getTaskName();
    String getTaskNameAsString();

    int readFromSensor(); // Returns a new reading from the photoresistor
    int getLightIntensity(); // Returns a value equal to the mean on multiple readings
    void setLightIntensity(int value);
};

/* Inline reference of the class used as reference inside
the classless function needed as *(void)(*) required to create
a task on a Core. */
inline static PhotoresistorReaderTask* prt;

inline static void PhotoresistorInitTask(void* parameter) {
    Serial.println(prt->getTaskNameAsString() + " is running on core " + String(xPortGetCoreID()));

    const TickType_t timer = WAIT_FOR_READ / portTICK_RATE_MS;

    for(;;) {
        prt->setLightIntensity(prt->readFromSensor());
        delay(timer);
    }
}

#endif