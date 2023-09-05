#include "Arduino.h"
#include "config.h"
#include "SmartGarden.h"
#include "Scheduler.h"
#include "Task.h"
#include "AutonomousStateTask.h"
#include "ManualStateTask.h"

Scheduler scheduler;
SmartGarden* smartGarden;
Task* autoTask;
Task* manualTask;

int leds[N_LEDS] = {LED_1, LED_2, LED_3, LED_4};

// AutonomousStateTask* automaticTester;
// ManualStateTask* manualTester;

void setup() {
    smartGarden = new SmartGarden();
    smartGarden->init(RX_PIN, TX_PIN);

    scheduler.init(10);

    autoTask = new AutonomousStateTask(leds, SERVO_PIN, smartGarden);
    autoTask->init(100);

    manualTask = new ManualStateTask(leds, SERVO_PIN, smartGarden);
    manualTask->init(100);

    scheduler.addTask(autoTask);
    scheduler.addTask(manualTask);
}

void loop() {
    scheduler.schedule();
}