#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display : public LiquidCrystal_I2C
{
public:
    Display(int lcd_addr, const uint8_t lcd_cols, const uint8_t lcd_rows) : LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows){};
    void print(uint8_t line, const char* s);
    void print(const char* s);
    void clear(uint8_t line);
    void clear();
};

#endif

