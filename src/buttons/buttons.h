#pragma once

#include <Arduino.h>

#ifndef BUTTONUP
#define BUTTONUP 35 
#endif
#ifndef BUTTONDOWN
#define BUTTONDOWN 33 
#endif
#ifndef BUTTONRIGHT
#define BUTTONRIGHT 26 
#endif
#ifndef BUTTONLEFT
#define BUTTONLEFT 34 
#endif
#ifndef BUTTONENTER
#define BUTTONENTER 14 
#endif

class Buttons
{
private:
    bool currentState;
    uint8_t pin;
    String name;
public:
    Buttons(uint8_t pin, String n);
    ~Buttons();

    void init();
    void update();
    bool state();
    String getName();
    
};

Buttons::Buttons(uint8_t pin, String n)
{
    this->name = n;
    this->pin = pin;
    this->currentState = LOW;
}

Buttons::~Buttons()
{
}

void Buttons::init()
{ 
    pinMode(pin,INPUT);
}

void Buttons::update()
{
    currentState = digitalRead(pin);
}

String Buttons::getName()
{
    return this->name;
}

bool Buttons::state()
{
    return this->currentState;
}
