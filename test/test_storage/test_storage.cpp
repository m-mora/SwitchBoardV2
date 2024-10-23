#include <unity.h>

#include "main.h"

ScheduleConf sch;

void test_saved_values()
{
    schedule_t s;

    s.days = 0xEF;
    s.hour = 12;
    s.minute = 30;
    s.duration = 600;
    sch.setConf(ZONE1,&s);
    // clear the values
    s = {};
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    // read values back
    s = sch.getConf(ZONE1);
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    TEST_ASSERT_EQUAL(0xEF,s.days);
    TEST_ASSERT_EQUAL(12,s.hour);
    TEST_ASSERT_EQUAL(30,s.minute);
    TEST_ASSERT_EQUAL(600,s.duration);
    s.days = 0xE0;
    s.hour = 21;
    s.minute = 9;
    s.duration = 1000;
    sch.setConf(ZONE2,&s);
    // clear the values
    s = {};
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    // read values back
    s = sch.getConf(ZONE2);
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    TEST_ASSERT_EQUAL(0xE0,s.days);
    TEST_ASSERT_EQUAL(21,s.hour);
    TEST_ASSERT_EQUAL(9,s.minute);
    TEST_ASSERT_EQUAL(1000,s.duration);

    s.days = 0x0F;
    s.hour = 1;
    s.minute = 1;
    s.duration = 20;
    sch.setConf(ZONE3,&s);
    // clear the values
    s = {};
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    // read values back
    s = sch.getConf(ZONE3);
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    TEST_ASSERT_EQUAL(0x0F,s.days);
    TEST_ASSERT_EQUAL(1,s.hour);
    TEST_ASSERT_EQUAL(1,s.minute);
    TEST_ASSERT_EQUAL(20,s.duration);

    s.days = 0xDD;
    s.hour = 23;
    s.minute = 0;
    s.duration = 200;
    sch.setConf(ZONE4,&s);
    // clear the values
    s = {};
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    // read values back
    s = sch.getConf(ZONE4);
    Serial.printf("Days = 0x%X, Hour = %d, Min = %d, Duration = %d\n",s.days,s.hour,s.minute,s.duration);
    TEST_ASSERT_EQUAL(0xDD,s.days);
    TEST_ASSERT_EQUAL(23,s.hour);
    TEST_ASSERT_EQUAL(0,s.minute);
    TEST_ASSERT_EQUAL(200,s.duration);

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
