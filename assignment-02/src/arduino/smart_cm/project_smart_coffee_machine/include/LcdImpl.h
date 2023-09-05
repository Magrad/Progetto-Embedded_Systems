#ifndef __LCD_IMPL__
#define __LCD_IMPL__

#include "Lcd.h"
#include "LiquidCrystal_I2C.h"
#include "config.h"

class LcdImpl : public Lcd
{

private:
    uint8_t address;
    uint8_t columns;
    uint8_t rows;

    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

public:
    LcdImpl(uint8_t adrress, uint8_t columns, uint8_t rows);
    void init();
    void backlight();
    void clear();
    void print(const char *msg);
    void setCursor(uint8_t x, uint8_t y);
};

#endif