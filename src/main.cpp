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
#define MAXBUTTONS  5



Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Buttons buttonUp(BUTTONUP,"ButtonUp");
Buttons buttonDown(BUTTONDOWN, "ButtonDown");
Buttons buttonRight(BUTTONRIGHT,"ButtonRight");
Buttons buttonLeft(BUTTONLEFT, "ButtonLeft");
Buttons buttonEnter(BUTTONENTER, "ButtonEnter");

Buttons button[]={buttonUp,buttonDown,buttonRight,buttonLeft,buttonEnter};

void setup()
{
    Serial.begin(115200);
    for(int index = 0 ; index < MAXBUTTONS; index++)
    {
        button[index].init();
    }
}

void loop()
{
    for(int index = 0 ; index < MAXBUTTONS; index++)
    {
        button[index].update();
        Serial.printf("Button pressed %d value %d\n",index, button[index].state());
    }
    delay(1000);
}

#endif
