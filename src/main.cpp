#ifndef PIO_UNIT_TESTING
#include "main.h"

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

Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BOARD_LED           2

void setup()
{
    Serial.begin(115200); 
    pinMode(2, OUTPUT);
}

void loop()
{
   // 
  digitalWrite(BOARD_LED, HIGH);
  
  // Keep the LED on for 5 seconds
  delay(1000);
  
  // Turn off the LED
  digitalWrite(BOARD_LED, LOW);

  delay(1000);

}

#endif
