#include "Arduino.h"
#include "config.h"
#include "SmartMachine.h"
#include "Scheduler.h"
#include "Task.h"
#include "WelcomeTask.h"
#include "UserInputTask.h"
#include "MakingDrinkTask.h"
#include "RetrieveDrinkTask.h"
#include "AssistanceModalityTask.h"
#include "IdleTask.h"

Scheduler scheduler;
Task *welcome;
Task *user;
Task *making;
Task *retrieve;
Task *assistance;
Task *idle;

void setup()
{
  Serial.begin(9600);
  SmartMachine::init(N_MAX);

  scheduler.init(200);

  welcome = new WelcomeTask();
  welcome->init(1000);
  user = new UserInputTask(BUT01_PIN, BUT02_PIN, BUT03_PIN, POT_PIN);
  user->init(50);
  making = new MakingDrinkTask(SERVO_PIN);
  making->init(1000);
  retrieve = new RetrieveDrinkTask(TRIG_PIN, ECHO_PIN, SERVO_PIN);
  retrieve->init(50);
  assistance = new AssistanceModalityTask(SERVO_PIN, DIGITAL_THERMO_PIN);
  assistance->init(1000);
  idle = new IdleTask(PIR_PIN);
  idle->init(50);
  scheduler.addTask(welcome);
  scheduler.addTask(user);
  scheduler.addTask(making);
  scheduler.addTask(retrieve);
  scheduler.addTask(assistance);
  scheduler.addTask(idle);
}

void loop()
{
  scheduler.schedule();
  SmartMachine::receiveMsg();
}
