#ifndef __SONAR_IMPL__
#define __SONAR_IMPL__

#include "Sonar.h"

class SonarImpl : public Sonar
{

private:
    int trigger;
    int echo;

public:
    SonarImpl(int trigger, int echo);
    float getDistance();
};

#endif