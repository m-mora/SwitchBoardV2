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
    display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, -1);
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
        display->setCursor(0, 0);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Up was pressed");
        display->display();
    }
    else
    {
        display->setCursor(0, 0);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Up was released ");
        display->display();
    }

    if (button_Down.pressed())
    {
        display->setCursor(0, 15);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Down was pressed");
        display->display();
    }
    else
    {
        display->setCursor(0, 15);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Down was released ");
        display->display();
    }

    if (button_Left.pressed())
    {
        display->setCursor(0, 30);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Left was pressed");
        display->display();
    }
    else
    {
        display->setCursor(0, 30);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Left was released ");
        display->display();
    }

    if (button_Right.pressed())
    {
        display->setCursor(0, 45);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Right was pressed");
        display->display();
    }
    else
    {
        display->setCursor(0, 45);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Right was released ");
        display->display();
    }

    if (button_Enter.pressed())
    {
        display->setCursor(0, 60);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Enter was pressed");
        display->display();
    }
    else
    {
        display->setCursor(0, 60);
        display->setTextColor(IDISPLAY_BLUE);
        display->setTextSize(1);
        display->print("Button Enter was released ");
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
