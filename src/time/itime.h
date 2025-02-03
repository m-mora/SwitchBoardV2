#pragma once

#include <time.h>
#include "ntp.h"
#include "rtc.h"

class ITime
{
private:
    ntp ntpTime;
    Rtc rtc;
    tm currentTime;
    int sec;
    int min;
    int hour;
    int day;
    int month;
    int year;
    bool valid;

    bool _update();

public:
    ITime();
    ~ITime();
    void init();
    bool getTime(uint8_t *hour, uint8_t *min, uint8_t *sec);
    bool getDate(uint8_t *day, uint8_t *month, uint16_t *year);
    tm getTimeDate();
    void showOled(Adafruit_SSD1306 *d);
    bool isValid();
};

ITime::ITime()
{
}

ITime::~ITime()
{
}

void ITime::init()
{
    ntpTime.init();
    rtc.init();
    if (ntpTime.getTime(&currentTime))
    {
        // NTP is working, update the RTC
        rtc.set(currentTime.tm_sec, currentTime.tm_min, currentTime.tm_hour, currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);
        rtc.printTime();
        this->valid = true;
    }
    else if (rtc.isValid())
    {
        Serial.println("Using RTC");
        this->valid = true;
    }
    else
    {
        this->valid = false;
    }
}

tm ITime::getTimeDate()
{
    this->_update();
    currentTime.tm_hour = this->hour;
    currentTime.tm_min = this->min;
    currentTime.tm_sec = this->sec;
    currentTime.tm_mday = this->day;
    currentTime.tm_mon = this->month;
    currentTime.tm_year = this->year - 1900;

    return currentTime;
}

bool ITime::_update()
{
    // define the priority, if the NTP is valid, let's use it
    if (ntpTime.isValid() && ntpTime.getTime(&currentTime))
    {
        this->hour = currentTime.tm_hour;
        this->min = currentTime.tm_min;
        this->sec = currentTime.tm_sec;
        this->day = currentTime.tm_mday;
        this->month = currentTime.tm_mon + 1;
        this->year = currentTime.tm_year + 1900;
        // NTP is working, update the RTC
        rtc.set(currentTime.tm_sec, currentTime.tm_min, currentTime.tm_hour, currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);
    }
    else if (rtc.isValid())
    {
        rtc.get(&this->sec, &this->min, &this->hour, &this->day, &this->month, &this->year);
    }
    else
    {
        // no clock valid
        return false;
    }

    return true;
}

bool ITime::getTime(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    if (this->_update())
    {
        *hour = this->hour;
        *min = this->min;
        *sec = this->sec;

        return true;
    }

    return false;
}

bool ITime::getDate(uint8_t *day, uint8_t *month, uint16_t *year)
{
    if (this->_update())
    {
        *day = this->day;
        *month = this->month;
        *year = this->year;
        return true;
    }
    return false;
}

void ITime::showOled(Adafruit_SSD1306 *d)
{
    this->_update();
    ntpTime.print_time_oled(d, &this->currentTime, true, false);
    rtc.printInOled(d, false);
}

bool ITime::isValid()
{
    return this->valid;
}
