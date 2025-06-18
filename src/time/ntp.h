#pragma once

#include <time.h>

#include "../connectivity/wifiConnection.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600; // offset to GMT in seconds, Mexico is -6 hours
const int daylightOffset_sec = 0;

class ntp
{
private:
  bool valid;

public:
  ntp();
  ~ntp();
  void init();
  int getTime(tm *t);
  bool isValid();
  void print_time_oled(Adafruit_SSD1306 *d, tm *c, bool clear = true, bool show = true);
};

void ntp::print_time_oled(Adafruit_SSD1306 *d, tm *c, bool clear, bool show)
{
  if (clear)
  {
    d->clearDisplay();
  }
  d->setTextColor(SSD1306_WHITE);
  d->setTextSize(2);
  d->setCursor(4, 1);
  char itime[128];
  strftime(itime, sizeof(itime), "%Y/%m/%d", c);
  d->println(itime);
  d->setCursor(17, 20);
  strftime(itime, sizeof(itime), "%H:%M:%S", c);
  d->println(itime);
  if (show)
  {
    d->display();
  }
}

ntp::ntp() {}

ntp::~ntp() {}
void ntp::init() { configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); }

int ntp::getTime(tm *t)
{
  if (!getLocalTime(t))
  {
    // Serial.println("fail to get the time");
    this->valid = false;
    return 0;
  }
  this->valid = true;
  // Serial.println(t, "%A, %B %d %Y %H:%M:%S");
  // Serial.print("Day of week: ");
  // Serial.println(t, "%A");
  // Serial.print("Month: ");
  // Serial.println(t, "%B");
  // Serial.print("Day of Month: ");
  // Serial.println(t, "%d");
  // Serial.print("Year: ");
  // Serial.println(t, "%Y");
  // Serial.print("Hour: ");
  // Serial.println(t, "%H");
  // Serial.print("Hour (12 hour format): ");
  // Serial.println(t, "%I");
  // Serial.print("Minute: ");
  // Serial.println(t, "%M");
  // Serial.print("Second: ");
  // Serial.println(t, "%S");

  // Serial.println("Time variables");
  // char timeHour[3];
  // strftime(timeHour, 3, "%H", t);
  // Serial.println(timeHour);
  // char timeWeekDay[10];
  // strftime(timeWeekDay, 10, "%A", t);
  // Serial.println(timeWeekDay);
  // Serial.println();

  return 1;
}

bool ntp::isValid()
{
  return this->valid;
}