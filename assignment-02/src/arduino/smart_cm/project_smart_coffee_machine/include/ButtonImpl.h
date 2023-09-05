#ifndef __BUTTON_IMPL__
#define __BUTTON_IMPL__

#include "Button.h"

class ButtonImpl : public Button
{

private:
  int pin;
  int state;

public:
  ButtonImpl(int pin);
  bool isPressed();
};

#endif
