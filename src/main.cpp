#ifndef PIO_UNIT_TESTING
#include "main.h"

// #define MAXBUTTONS 5

// Buttons buttonUp(BUTTONUP, "Up");
// Buttons buttonDown(BUTTONDOWN, "Down");
// Buttons buttonRight(BUTTONRIGHT, "Right");
// Buttons buttonLeft(BUTTONLEFT, "Left");
// Buttons buttonEnter(BUTTONENTER, "Enter");

// Buttons button[] = {buttonUp, buttonDown, buttonRight, buttonLeft, buttonEnter};

IDisplay *display;

void setup()
{
    Serial.begin(115200);
    display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
    display->clearScreen();
    display->display();
    setup_buttons();
    // for (int index = 0; index < MAXBUTTONS; index++)
    // {
    //     button[index].init();
    // }
}

void loop()
{
    update_buttons();

    if (button_Up.pressed())
    {
            display->setCursor(0,0);
    display->setTextColor(IDISPLAY_BLUE);
    display->setTextSize(i);
    display->print("Text size");
    display->print(i);
    display->display();
    }
    // for (int index = 0; index < MAXBUTTONS; index++)
    // {
    //     button[index].update();
    //     if (button[index].state() == HIGH)
    //     {
    //         Serial.printf("Button pressed %s value %d\n", button[index].getName(), button[index].state());
    //     }
    // }
    delay(100);
}

#endif
