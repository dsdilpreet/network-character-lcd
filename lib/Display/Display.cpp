#include "Display.h"

uint8_t m_lcd_cols = 0; 
uint8_t m_lcd_rows = 0;

 // max number of rows for common character lcds
const char* m_lcd_data[4];
int m_lcd_data_length[4];

uint8_t m_is_autoscroll_on = 0;

Display::Display(int lcd_addr, const uint8_t lcd_cols, const uint8_t lcd_rows) : 
LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows)
{
    m_lcd_cols = lcd_cols;
    m_lcd_rows = lcd_rows;

    // clear and init the display
    for (uint8_t i = 0; i < m_lcd_rows; i++)
    {
      printText(i, " ");
    }
}

/* enum TextAlign
{
    Left,
    Center,
    Right
}; */

void Display::printText(uint8_t row, const char* text, uint8_t textAlign)
{
    // set text
    m_lcd_data[row] = text;
    m_lcd_data_length[row] = strlen(text);

    // clear any old data on specified row
    clearText(row);

    uint8_t startingIndex = 0;
    // print data on specified row
    switch (textAlign)
    {
    case 1: // center align
      if ((strlen(text) <= m_lcd_cols - 2) && strlen(text) > 0)
      {
        startingIndex = (m_lcd_cols - strlen(text)) / 2;
      }
      break;
    case 2: // right align
      if ((strlen(text) <= m_lcd_cols - 1) && strlen(text) > 0)
      {
        startingIndex = m_lcd_cols - strlen(text);
      }
    }
    
    LiquidCrystal_I2C::setCursor(startingIndex,row);

    // So correct number of characters cam be printed on line based on the maximum characters per line
    char cols[10];
    cols[0] = '\0';
    itoa(m_lcd_cols, cols, 10);

    char result[10];
    result[0] = '\0';
    strcat(result, "%.");
    strcat(result, cols);
    strcat(result, "s");

    LiquidCrystal_I2C::printf(result, m_lcd_data[row]);
}

// Pass in the index of row to clear it.
void Display::clearText(uint8_t row)
{
  for (uint8_t i = 0; i < m_lcd_cols; i++)
  {
    LiquidCrystal_I2C::setCursor(i,row);
    LiquidCrystal_I2C::print(" ");
  }
}

void Display::setScroll(uint8_t status)
{
    m_is_autoscroll_on = status;
}

void Display::scroll()
{

  if (!m_is_autoscroll_on)
  {
    return;
  }

  for (uint8_t i = 0; i < m_lcd_rows; i++)
  {

    // if the length of text is less than number of columns, then do not scroll.
    if (m_lcd_data_length[i] < m_lcd_cols)
    {
        continue;
    }

    if(strlen(m_lcd_data[i]) > 0)
    {
      for (uint8_t j = 0; j < m_lcd_cols; j++)
      {
        LiquidCrystal_I2C::setCursor(j,i);
        LiquidCrystal_I2C::print(" ");
      }     
      LiquidCrystal_I2C::setCursor(0, i);
      // So correct number of characters cam be printed on line based on the maximum characters per line
      char cols[10];
      cols[0] = '\0';
      itoa(m_lcd_cols, cols, 10);

      char result[10];
      result[0] = '\0';
      strcat(result, "%.");
      strcat(result, cols);
      strcat(result, "s");

      LiquidCrystal_I2C::printf(result, m_lcd_data[i]);
      m_lcd_data[i]++;
    }
    else
    {
      m_lcd_data[i] = m_lcd_data[i] - m_lcd_data_length[i];
    }
  }
}