#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

class Rtc
{
private:
    RTC_DS1307 rtc;
    DateTime now;
    bool valid;

public:
    Rtc();
    ~Rtc();
    int init();
    void set(int sec, int min, int hour, int day, int month, int year);
    void get(int *sec, int *min, int *hour, int *day, int *month, int *year);
    bool isValid();
    void printTime();
    void printInOled(Adafruit_SSD1306 *d, bool clear = true, bool show = true);
};

Rtc::Rtc()
{
}

Rtc::~Rtc()
{
}

int Rtc::init()
{
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        this->valid = false;
        return false;
    }
    this->valid = true;
    return true;
}

void Rtc::set(int sec, int min, int hour, int day, int month, int year)
{
    // (year, month, day, hour, min, sec)
    rtc.adjust(DateTime(year, month, day, hour, min, sec));
}

void Rtc::get(int *sec, int *min, int *hour, int *day, int *month, int *year)
{

    now = rtc.now();

    *year = now.year();
    *month = now.month();
    *day = now.day();
    *hour = now.hour();
    *min = now.minute();
    *sec = now.second();
}

void Rtc::printTime()
{
    now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.println(now.day(), DEC);
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
}

void Rtc::printInOled(Adafruit_SSD1306 *d, bool clear, bool show)
{
    char buff[50];

    // get time from RTC
    now = rtc.now();

    if (clear)
    {
        d->clearDisplay();
    }
    d->setTextColor(SSD1306_WHITE);
    d->setTextSize(1);
    d->setCursor(35, 36);
    sprintf(buff, "%02d/%02d/%04d", now.day(), now.month(), now.year());
    d->println(buff);
    d->setCursor(40, 48);
    sprintf(buff, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    d->println(buff);
    if (show)
    {
        d->display();
    }
}

bool Rtc::isValid()
{
    this->valid = rtc.isrunning();
    return this->valid;
}