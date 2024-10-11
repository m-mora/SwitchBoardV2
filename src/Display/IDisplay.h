#pragma once

#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789

#include "DisplayConsts.h"


class IDisplay 
{
private:
  /* data */
public:
  virtual void init(uint8_t address = DISPLAY_ADDR);
  virtual void setCursor(int16_t x, int16_t y);
  virtual void clearScreen();
  virtual void showLogo();
  virtual uint16_t width();
  virtual uint16_t height();
  virtual void drawLine(uint16_t width, uint16_t height, uint16_t x, uint16_t y, uint16_t color);
  virtual void display();
};



