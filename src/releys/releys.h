#pragma once

#include <Arduino.h>

#ifndef RELEY1
#define RELEY1 27
#endif
#ifndef RELEY2
#define RELEY2 32
#endif
#ifndef RELEY3
#define RELEY3 12
#endif
#ifndef RELEY4
#define RELEY4 4
#endif
#ifndef RELEY_POWER
#define RELEY_POWER 2
#endif

#define TURN_ON_POWER LOW  // this has inverse logic
#define TURN_OFF_POWER HIGH
#define TURN_ON HIGH
#define TURN_OFF LOW

enum
{
    reley1,
    reley2,
    reley3,
    reley4,
    reley_power,
    MAX_RELEY
};

class Reley
{
private:
    bool statusOn[5];
    uint8_t reley_pin[5];
    unsigned long timeOn[5];
    bool anyRelayIsOn;

public:
    Reley();
    Reley(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t rp);
    ~Reley();
    void init();
    bool turnOn(uint8_t reley);
    bool turnOff(uint8_t reley);
    bool isOn(uint8_t reley);
    uint32_t timeOninSeconds(uint8_t zone);
    bool isAnyRelayOn();
};

Reley::Reley()
{
    this->reley_pin[reley1] = RELEY1;
    this->reley_pin[reley2] = RELEY2;
    this->reley_pin[reley3] = RELEY3;
    this->reley_pin[reley4] = RELEY4;
    this->reley_pin[reley_power] = RELEY_POWER;
}

Reley::Reley(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t rp)
{
    this->reley_pin[reley1] = r1;
    this->reley_pin[reley2] = r2;
    this->reley_pin[reley3] = r3;
    this->reley_pin[reley4] = r4;
    this->reley_pin[reley_power] = rp;
}

void Reley::init()
{
    for (int i = 0; i < MAX_RELEY - 1; i++)
    {
        pinMode(reley_pin[i], OUTPUT);
        digitalWrite(reley_pin[i], TURN_OFF);
        this->statusOn[i] = false;
    }
    pinMode(reley_pin[reley_power], OUTPUT);
    digitalWrite(reley_pin[reley_power], TURN_OFF_POWER);
    this->statusOn[reley_power] = false;
    this->anyRelayIsOn = false;
    // Serial.print("GPIO0 = ");
    // Serial.println(digitalRead(RELEY_POWER));
}

Reley::~Reley()
{
}

bool Reley::turnOn(uint8_t reley)
{
    if (this->anyRelayIsOn)
    {
        // can not turn on a reley because there is one ON
        // And only one must be On at the same time
        return false;
    }
    if (reley < MAX_RELEY - 1)
    {
        if (!this->statusOn[reley])
        {
            // turn on the source of power
            digitalWrite(reley_pin[reley_power], TURN_ON_POWER);
            digitalWrite(reley_pin[reley], TURN_ON);
            this->statusOn[reley] = true;
        }
        this->timeOn[reley] = millis();
        this->anyRelayIsOn = true;
        // Serial.print("GPIO0 = ");
        // Serial.println(digitalRead(RELEY_POWER));

        return true;
    }
    return false;
}

bool Reley::turnOff(uint8_t reley)
{
    if (reley < MAX_RELEY - 1)
    {
        if (this->statusOn[reley])
        {
            digitalWrite(reley_pin[reley], TURN_OFF);            // turn off the reley
            digitalWrite(reley_pin[reley_power], TURN_OFF_POWER);      // turn off the power reley
            this->statusOn[reley] = false;
            this->anyRelayIsOn = false;
        }

        return true;
    }
    return false;
}

bool Reley::isOn(uint8_t reley)
{
    if (reley < MAX_RELEY - 1)
    {
        return this->statusOn[reley];
    }
    // retunr false if the reley parameter is not valid
    return false;
}

uint32_t Reley::timeOninSeconds(uint8_t zone)
{
    return (uint32_t)(millis() - this->timeOn[zone]) / 1000;
}

bool Reley::isAnyRelayOn()
{
    return this->anyRelayIsOn;
}
