#pragma once

#include <Preferences.h> // this functions are to use the EEPROM

#define SUNDAY BIT6
#define MONDAY BIT5
#define TUESDAY BIT4
#define WEDNESDAY BIT3
#define THURSDAY BIT2
#define FRIDAY BIT1
#define SATURDAY BIT0
#define ALLWEEK (SUNDAY|MONDAY|TUESDAY|WEDNESDAY|THURSDAY|FRIDAY|SATURDAY)

enum
{
    ZONE1 = 0,
    ZONE2,
    ZONE3,
    ZONE4,
    MAX_ZONES
} zones_e;

/*
 * Day: NA | Sun | Mon | Tues | wed | thur | Fri | Sat |
 *       0    1     0      1     0     1      0     0   = 0x54
 * Hour: 24 format
 * Minutes:
 * Duration: in seconds
 */
typedef struct
{
    uint8_t days;
    uint8_t hour;
    uint8_t minute;
    uint16_t duration;
} schedule_t;

class ScheduleConf
{
private:
    Preferences *store;
    String _zoneNames[MAX_ZONES] = {"zone1", "zone2", "zone3", "zone4"};
    // these are the default values if not previous configuration was done.
    schedule_t _zones[MAX_ZONES] = {{0x54, 23, 0, 120}, {0x54, 23, 5, 120}, {0x54, 23, 10, 120}, {0x54, 23, 15, 120}};
    String _nameSpace = "schedule";

public:
    ScheduleConf();
    ~ScheduleConf();

    void init(void);
    schedule_t getConf(int zone);
    void setConf(int zone, schedule_t *conf);
};
