#include "Display.h"

// Display::Display(int lcd_addr, const uint8_t lcd_cols, const uint8_t lcd_rows) : LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows)
// {

// }

void Display::print(const char* s)
{
    LiquidCrystal_I2C::print(s);
}

void Display::clear()
{
    LiquidCrystal_I2C::clear();
}