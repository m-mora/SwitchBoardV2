#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#include "Storage/scheduleconf.h"
#include "Display/DisplayConsts.h"
//#include "Storage/mode.h"
// #include "Display/IDisplay.h"
// #include "Display/DisplaySSD1306.h"
// #include "Display/DisplayST7735.h"
#include "buttons/keyboard.h"
#include "Menu/menu.h"

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

#include <Wire.h>

void ScanI2CDevicesAndDumpTable()
{
 byte error, address;
  int nDevices;
  Wire.begin(21,22);
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);       }