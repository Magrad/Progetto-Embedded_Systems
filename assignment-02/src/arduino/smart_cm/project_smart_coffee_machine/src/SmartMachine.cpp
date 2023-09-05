#include "SmartMachine.h"
#include "EnableInterrupt.h"
#include "MsgService.h"

// Lcd display used to print out message for the users
static Lcd *lcd;

// Name of the drinks available in the Smart Machine
static const char *selected_drink[] = {COFFEE_MSG, TEA_MSG, CHOCOLATE_MSG};

// Current number of drinks available per beverage
int SmartMachine::products[MAX - 1] = {};

// Current beverage selected
int SmartMachine::selected = MIN;

// Current moving direction for drinks to display
int SmartMachine::dir = 1;

// Current drinks availability state of the machine
bool SmartMachine::empty = false;

// Current machine state
StateType SmartMachine::state = WELCOME;

// Current sugar level selected with the potentiometer
int SmartMachine::sugarLevel = 0;

// Current number of Machine tests executed
int SmartMachine::nTests = 0;

// Time of the last Machine tests executed
long SmartMachine::lastTest = 0;

/**
 * @brief Initialize Smart Machine's variables
 *
 * @param n_drinks Number of maximun drinks available per beverage
 */
void SmartMachine::init(int n_drinks)
{
    enableInterrupt(PIR_PIN, doNothing, CHANGE);
    MsgService.init();
    lcd = new LcdImpl(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

    lcd->init();
    lcd->backlight();

    refill(n_drinks);
}

/**
 * @brief Sends a message to the Java application
 *
 * @param msg String to be sent
 */
void SmartMachine::sendMsg(const String msg)
{
    MsgService.sendMsg(msg);
}

/**
 * @brief Checks if the Java application is asking for
 * data, if the message fulfill the criteria some action
 * will be taken by the Smart Machine
 */
void SmartMachine::receiveMsg()
{
    if (MsgService.isMsgAvailable())
    {
        Msg *msg = MsgService.receiveMsg();

        if (msg->getContent() == FILL)
        {
            // Refills the machine
            refill(N_MAX);

            if(empty) {
                state = WELCOME;
            }
        }
        if (msg->getContent() == RESTART)
        {
            // Restarts the machine if assistance was required
            state = WELCOME;
        }

        delete msg;
    }
}

/**
 * @brief Changes the current beverage to display
 *
 * @param direction The direction of wich beverage to show next
 */
void SmartMachine::changeSelected(int direction)
{
    dir = direction;
    selected += direction;
    // Checks if the selection is out bound, if so acts like a linked list
    selected = selected <= MIN ? MAX - 1 : selected >= MAX ? MIN + 1
                                                           : selected;
    displayClear();
    updateDisplay();
}

/**
 * @brief Displays the message that needs to be printed on the lcd
 *
 * @param text Message that wants to be displayed
 * @param x Indentation of the message. From left (0) to right
 * @param y Row number
 */
void SmartMachine::displayPrint(char *text, int x, int y)
{
    lcd->setCursor(x, y);
    lcd->print(text);
}

/**
 * @brief Clears the display from the old text that was shown
 */
void SmartMachine::displayClear()
{
    lcd->clear();
}

/**
 * @brief Called every time the user wants to change selected beverage.
 */
void SmartMachine::updateDisplay()
{
    empty = 1;

    // Checks if the machine is out of beverage
    for (int i = 0; i < int(sizeof(products) / sizeof(products[0])); i++)
    {
        empty = products[i] > 0 ? 0 : empty;
    }

    if (!empty)
    {
        for (int i = 0; i < int(sizeof(products) / sizeof(products[0])); i++)
        {
            // Checks if the next beverage that will be shown is available or not
            // if so skips one ahead
            if (products[i] == 0 && i == selected - 1)
            {
                changeSelected(dir);
            }
        }

        // Displays the correct beverage
        displayPrint(selected_drink[selected - 1], 0, 0);

        // Displays the current sugar level selected for the beverage
        char text[20];
        String sugar = String(sugarLevel);

        strcpy(text, "Sugar: ");
        strcat(text, sugar.c_str());
        displayPrint(text, 0, 3);
    }
    else
    {
        // Machine out of beverage; wait for an Admin to refill from the Java application
        state = MACHINE_ASSISTANCE_MODALITY;
    }
}

/**
 * @brief Displays that the drinks is in the making process
 */
void SmartMachine::makingDrink()
{
    char text[20];
    strcpy(text, MAKING_MSG);
    strcat(text, selected_drink[selected - 1]);

    displayPrint(text, 0, 0);
}

/**
 * @brief Displays that the drinks is ready for the user to pick enjoy
 */
void SmartMachine::drinkReady()
{
    products[selected - 1]--;
    sendMsg(String(selected_drink[selected - 1]) + " " + String(products[selected - 1]));

    char text[20];
    strcpy(text, THE_MSG);
    strcat(text, selected_drink[selected - 1]);
    strcat(text, DRINK_READY_MSG);

    displayPrint(text, 0, 0);
}

/**
 * @brief Resets the current beverage selected
 */
void SmartMachine::reset()
{
    selected = MIN;
}

/**
 * @brief Refills the Smart Machine and sends to the Java application
 * the current number of drinks available
 * @param n_drinks Number of maximun drinks available per beverage
 */
void SmartMachine::refill(int n_drinks)
{
    for (int i = 0; i < int(sizeof(products) / sizeof(products[0])); i++)
    {
        products[i] = n_drinks;
    }

    sendMsg(String(FILL) + "" + String(N_MAX));
}

void SmartMachine::doNothing()
{
    // DO NOTHING
}

/**
 * @brief Prints the message that wants to be displayed on the Serial Line
 * used to debug portion of code
 * @param msg Message that wants to be displayed
 */
void SmartMachine::debug(char *msg)
{
    Serial.println(msg);
}