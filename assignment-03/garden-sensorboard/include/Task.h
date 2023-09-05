#ifndef __TASK_H__
#define __TASK_H__

#include "Arduino.h"

class Task {

public:
    virtual void init(); // Starts the task on a given core of the ESP32
    virtual const char* getTaskName();  // Returns the task name as a char
    virtual String getTaskNameAsString(); // Returns the task name as a String
};

#endif