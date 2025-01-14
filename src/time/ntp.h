#pragma once

#include <time.h>

#include "../connectivity/wifiConnection.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600;
const int daylightOffset_sec = 0;

class ntp {
 private:
  /* data */
 public:
  ntp();
  ~ntp();
  void init();
  int getTime(tm* t);
};

ntp::ntp() {}

ntp::~ntp() {}
void ntp::init() { configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); }

int ntp::getTime(tm* t) {
  if (!getLocalTime(t)) {
    Serial.println("fail to get the time");
    return 0;
  }
  Serial.println(t, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(t, "%A");
  Serial.print("Month: ");
  Serial.println(t, "%B");
  Serial.print("Day of Month: ");
  Serial.println(t, "%d");
  Serial.print("Year: ");
  Serial.println(t, "%Y");
  Serial.print("Hour: ");
  Serial.println(t, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(t, "%I");
  Serial.print("Minute: ");
  Serial.println(t, "%M");
  Serial.print("Second: ");
  Serial.println(t, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour, 3, "%H", t);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", t);
  Serial.println(timeWeekDay);
  Serial.println();

  return 1;
}