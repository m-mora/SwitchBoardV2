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

Bounce2::Button button_Up = Bounce2::Button();
Bounce2::Button button_Down = Bounce2::Button();
Bounce2::Button button_Right = Bounce2::Button();
Bounce2::Button button_Left = Bounce2::Button();
Bounce2::Button button_Enter = Bounce2::Button();

void setup_buttons()
{
    // Buttons setup
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

// class Buttons
// {
// private:
//     bool currentState;
//     uint8_t pin;
//     String name;
//     uint8_t lastState;
//     uint32_t lastDeboudeTime;

// public:
//     Buttons(uint8_t pin, String n);
//     ~Buttons();

//     void init();
//     void update();
//     bool state();
//     String getName();
// };

// Buttons::Buttons(uint8_t pin, String n)
// {
//     this->name = n;
//     this->pin = pin;
//     this->currentState = LOW;
//     this->lastState = LOW;
//     this->lastDeboudeTime = 0;
// }

// Buttons::~Buttons()
// {
// }

// void Buttons::init()
// {
//     pinMode(pin, INPUT);
// }

// void Buttons::update()
// {
//     // this->currentState = digitalRead(pin);
//     int valueRead = digitalRead(pin);

//     // Check if the button state has changed
//     if (valueRead != this->lastState)
//     {
//         this->lastDeboudeTime = millis(); // Reset the debounce timer
//     }

//     // Only update the button state if the debounce time has passed
//     if ((millis() - this->lastDeboudeTime) > DEBOUNCE_DELAY)
//     {
//         // Check if the button state is stable and different from the last known state
//         if (valueRead != this->currentState)
//         {
//             this->currentState = valueRead;
//         }
//     }
// }

// String Buttons::getName()
// {
//     return this->name;
// }

// bool Buttons::state()
// {
//     return this->currentState;
// }
