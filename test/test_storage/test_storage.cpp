#include <unity.h>

#include "main.h"

ScheduleConf sch;

void test_saved_values()
{
    schedule_t s;
    uint8_t days[MAX_ZONES] = {(ALLWEEK), (MONDAY | WEDNESDAY | FRIDAY), (TUESDAY | THURSDAY | SATURDAY), (SUNDAY | WEDNESDAY | SATURDAY)};
    uint8_t hour[MAX_ZONES] = {12, 21, 1, 23};
    uint8_t minutes[MAX_ZONES] = {30, 9, 1, 0};
    uint16_t duration[MAX_ZONES] = {600, 1000, 20, 250};
    bool humidity = false;
    bool rain = false;
    uint8_t pir = 0;

    for (int i = ZONE1; i < MAX_ZONES; i++)
    {
        s.days = days[i];
        s.hour = hour[i];
        s.minute = minutes[i];
        s.duration = duration[i];
        s.humidity = humidity;
        s.rain = rain;
        s.pir = pir;
        sch.setConf(i, &s);
        // clear the values
        s = {};
        Serial.printf("Zone = %d, Days = 0x%02X, Hour = %02d, Min = %02d, Duration = %d\n", i, s.days, s.hour, s.minute, s.duration);
        // read values back
        s = sch.getConf(i);
        Serial.printf("Zone = %d, Days = 0x%02X, Hour = %02d, Min = %02d, Duration = %d\n", i, s.days, s.hour, s.minute, s.duration);
        TEST_ASSERT_EQUAL(days[i], s.days);
        TEST_ASSERT_EQUAL(hour[i], s.hour);
        TEST_ASSERT_EQUAL(minutes[i], s.minute);
        TEST_ASSERT_EQUAL(duration[i], s.duration);
        TEST_ASSERT_EQUAL(humidity, s.humidity);
        TEST_ASSERT_EQUAL(rain, s.rain);
        TEST_ASSERT_EQUAL(pir, s.pir);
    }
    return;
}
//----------------
void setup()
{
    Serial.begin(115200);

    UNITY_BEGIN();
    sch.init();
    RUN_TEST(test_saved_values);

    UNITY_END();
}

void loop()
{
}
