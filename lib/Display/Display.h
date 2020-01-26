#ifndef Display_h
#define Display_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

enum TextAlign
{
    Left,
    Center,
    Right
};

class Display : public LiquidCrystal_I2C
{
public:
    Display(int lcd_addr, const uint8_t lcd_cols, const uint8_t lcd_rows);
    void printText(uint8_t row, const char* text, int textAlign = 0);
    void scroll();
    void setScroll(uint8_t status);
private:
    void clearText(uint8_t row);
};

#endif

