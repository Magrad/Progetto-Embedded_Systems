#ifndef __SMART_MACHINE__
#define __SMART_MACHINE__

#include "Arduino.h"
#include "LcdImpl.h"
#include "TypeState.h"
#include "config.h"

class SmartMachine
{

private:
    static enum { MIN,
                  COFFEE,
                  TEA,
                  CHOCOLATE,
                  MAX } drink;
    static int products[MAX - 1];
    static int selected;
    static int dir;
    static bool empty;

    static void refill(int n_drinks);
    static void doNothing();

public:
    static void init(int n_drinks);
    static void sendMsg(const String msg);
    static void receiveMsg();
    static void changeSelected(int direction);
    static void displayPrint(char *text, int x, int y);
    static void displayClear();
    static void updateDisplay();
    static void makingDrink();
    static void drinkReady();
    static void reset();
    static StateType state;
    static int sugarLevel;
    static int nTests;
    static long lastTest;

    static void debug(char *msg);
};

#endif