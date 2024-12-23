#pragma once

#include <Arduino.h>
#include <Bounce2.h>

#ifndef BUTTONUP
#define BUTTONUP 35
#endif
#ifndef BUTTONDOWN
#define BUTTONDOWN 33
#endif
#ifndef BUTTONRIGHT
#define BUTTONRIGHT 34
#endif
#ifndef BUTTONLEFT
#define BUTTONLEFT 26
#endif
#ifndef BUTTONENTER
#define BUTTONENTER 14
#endif
#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 5 // Time in miliseconds
#endif

enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    MAX_BUTTONS
};

// Create the instace for each button
Bounce2::Button button_Up = Bounce2::Button();
Bounce2::Button button_Down = Bounce2::Button();
Bounce2::Button button_Right = Bounce2::Button();
Bounce2::Button button_Left = Bounce2::Button();
Bounce2::Button button_Enter = Bounce2::Button();

void setup_buttons()
{
    // Buttons setup

    // Set the pin connected to the button
    button_Up.attach(BUTTONUP, INPUT);
    button_Down.attach(BUTTONDOWN, INPUT);
    button_Right.attach(BUTTONRIGHT, INPUT);
    button_Left.attach(BUTTONLEFT, INPUT);
    button_Enter.attach(BUTTONENTER, INPUT);

    // Debounce interval in miliseconds
    button_Up.interval(DEBOUNCE_DELAY);
    button_Down.interval(DEBOUNCE_DELAY);
    button_Right.interval(DEBOUNCE_DELAY);
    button_Left.interval(DEBOUNCE_DELAY);
    button_Enter.interval(DEBOUNCE_DELAY);

    // Indicate that the high state corresponds to physically pressing button
    button_Up.setPressedState(HIGH);
    button_Down.setPressedState(HIGH);
    button_Right.setPressedState(HIGH);
    button_Left.setPressedState(HIGH);
    button_Enter.setPressedState(HIGH);
}

void update_buttons()
{
    button_Up.update();
    button_Down.update();
    button_Right.update();
    button_Left.update();
    button_Enter.update();
}
