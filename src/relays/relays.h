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

#define TURN_ON_POWER LOW // this has inverse logic
#define TURN_OFF_POWER HIGH
#define TURN_ON HIGH
#define TURN_OFF LOW

enum
{
    relay1,
    relay2,
    relay3,
    relay4,
    relay_power,
    MAX_RELEY
};

class Reley
{
private:
    bool statusOn[5];
    uint8_t relay_pin[5];
    unsigned long timeOn[5];
    bool anyRelayIsOn;

public:
    Reley();
    Reley(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t rp);
    ~Reley();
    void init();
    bool turnOn(uint8_t relay);
    bool turnOff(uint8_t relay);
    bool isOn(uint8_t relay);
    uint32_t timeOninSeconds(uint8_t zone);
    bool isAnyRelayOn();
};

Reley::Reley()
{
    this->relay_pin[relay1] = RELEY1;
    this->relay_pin[relay2] = RELEY2;
    this->relay_pin[relay3] = RELEY3;
    this->relay_pin[relay4] = RELEY4;
    this->relay_pin[relay_power] = RELEY_POWER;
}

Reley::Reley(uint8_t r1, uint8_t r2, uint8_t r3, uint8_t r4, uint8_t rp)
{
    this->relay_pin[relay1] = r1;
    this->relay_pin[relay2] = r2;
    this->relay_pin[relay3] = r3;
    this->relay_pin[relay4] = r4;
    this->relay_pin[relay_power] = rp;
}

void Reley::init()
{
    for (int i = 0; i < MAX_RELEY - 1; i++)
    {
        pinMode(relay_pin[i], OUTPUT);
        digitalWrite(relay_pin[i], TURN_OFF);
        this->statusOn[i] = false;
    }
    pinMode(relay_pin[relay_power], OUTPUT);
    digitalWrite(relay_pin[relay_power], TURN_OFF_POWER);
    this->statusOn[relay_power] = false;
    this->anyRelayIsOn = false;
    // Serial.print("GPIO0 = ");
    // Serial.println(digitalRead(RELEY_POWER));
}

Reley::~Reley()
{
}

bool Reley::turnOn(uint8_t relay)
{
    if (this->anyRelayIsOn)
    {
        // can not turn on a relay because there is one ON
        // And only one must be On at the same time
        return false;
    }
    if (relay < MAX_RELEY - 1)
    {
        if (!this->statusOn[relay])
        {
            // turn on the source of power
            digitalWrite(relay_pin[relay_power], TURN_ON_POWER);
            digitalWrite(relay_pin[relay], TURN_ON);
            this->statusOn[relay] = true;
        }
        this->timeOn[relay] = millis();
        this->anyRelayIsOn = true;
        // Serial.print("GPIO0 = ");
        // Serial.println(digitalRead(RELEY_POWER));

        return true;
    }
    return false;
}

bool Reley::turnOff(uint8_t relay)
{
    if (relay < MAX_RELEY - 1)
    {
        if (this->statusOn[relay])
        {
            digitalWrite(relay_pin[relay], TURN_OFF);             // turn off the relay
            digitalWrite(relay_pin[relay_power], TURN_OFF_POWER); // turn off the power relay
            this->statusOn[relay] = false;
            this->anyRelayIsOn = false;
        }

        return true;
    }
    return false;
}

bool Reley::isOn(uint8_t relay)
{
    if (relay < MAX_RELEY - 1)
    {
        return this->statusOn[relay];
    }
    // retunr false if the relay parameter is not valid
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
