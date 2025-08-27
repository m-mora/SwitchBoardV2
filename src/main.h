#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "tools/i2cScan.h"
#include "Display/DisplayConsts.h"
#include "Storage/scheduleconf.h"
#include "Menu/menu.h"
#include "buttons/keyboard.h"
#include "connectivity/wifiConnection.h"
#include "time/itime.h"
#include "relays/relays.h"

// Configure the pins connected to the SPI port
#ifndef SPI_SCK
#define SPI_SCK 18
#endif
#ifndef SPI_MISO
#define SPI_MISO 19 // Data command
#endif
#ifndef SPI_DC
#define SPI_DC 19 // Data Command
#endif
#ifndef SPI_MOSI
#define SPI_MOSI 23
#endif
#ifndef SPI_CS
#define SPI_CS 5
#endif

// Configure OLED
#ifndef DISPLAY_ADDR
#define DISPLAY_ADDR 0x3C // Most common address used for Oled displays
#endif
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#endif
#ifndef OLED_RESET
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#endif
