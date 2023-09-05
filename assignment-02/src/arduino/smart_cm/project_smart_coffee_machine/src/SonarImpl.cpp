#include "SonarImpl.h"
#include "Arduino.h"

#define TEMPERATURE 20

SonarImpl::SonarImpl(int trigger, int echo)
{
    this->trigger = trigger;
    this->echo = echo;
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
}

float SonarImpl::getDistance()
{
    digitalWrite(trigger, LOW);
    delayMicroseconds(3);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigger, LOW);

    const float vs = 331.45 + 0.62 * TEMPERATURE;
    float tUS = pulseIn(echo, HIGH);
    float t = tUS / 1000.0 / 1000.0 / 2;
    return t * vs;
}