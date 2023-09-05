#include "LcdImpl.h"

LcdImpl::LcdImpl(uint8_t adrress, uint8_t columns, uint8_t rows)
{
    this->address = address;
    this->columns = columns;
    this->rows = rows;
}

void LcdImpl::init()
{
    lcd.init();
}

void LcdImpl::backlight()
{
    lcd.backlight();
}

void LcdImpl::clear()
{
    lcd.clear();
}

void LcdImpl::setCursor(uint8_t x, uint8_t y)
{
    lcd.setCursor(x, y);
}

void LcdImpl::print(const char *msg)
{
    lcd.print(msg);
}