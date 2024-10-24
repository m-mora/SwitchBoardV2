#pragma once

#include <Preferences.h>

#define MAX_ZONES 4
enum
{
    ZONE1 = 0,
    ZONE2,
    ZONE3,
    ZONE4
} zones_e;

typedef struct
{
    uint8_t days;
    uint8_t hour;
    uint8_t minute;
    uint16_t duration;
} schedule_t;

/*
 * Day: NA | Sun | Mon | Tues | wed | thur | Fri | Sat |
 *       0    1     0      1     0     1      0     0   = 0x54
 * Hour: 24 format
 * Minutes:
 * Duration: in seconds
 */
schedule_t defaultZone1 = {0x54, 23, 0, 120};
schedule_t defaultZone2 = {0x54, 23, 5, 120};
schedule_t defaultZone3 = {0x54, 23, 10, 120};
schedule_t defaultZone4 = {0x54, 23, 15, 120};

class ScheduleConf
{
private:
    Preferences *store;
    String _zoneNames[MAX_ZONES] = {"zone1", "zone2", "zone3", "zone4"};
    schedule_t _zones[MAX_ZONES] = {defaultZone1, defaultZone2, defaultZone3, defaultZone4};
    String _nameSpace = "schedule";

public:
    ScheduleConf() {};
    ~ScheduleConf() {};

    void init(void);
    schedule_t getConf(int zone);
    void setConf(int zone, schedule_t *conf);
};

void ScheduleConf::init()
{
    store = new Preferences();
    store->begin(_nameSpace.c_str(), false);
    // Check if it is the first time it runs and there is not data saved.
    if (!store->isKey(this->_zoneNames[0].c_str()))
    {
        // if not data saved, save default values
        for (int i = 0; i < MAX_ZONES; i++)
        {
            store->putBytes(this->_zoneNames[i].c_str(), &this->_zones[i], sizeof(schedule_t));
        }
    }
    // close the namespace in case some other code wants to save data in a different one.
    store->end();
}

schedule_t ScheduleConf::getConf(int zone)
{
    schedule_t result;
    store->begin(_nameSpace.c_str());
    store->getBytes(this->_zoneNames[zone].c_str(), &result, sizeof(schedule_t));
    store->end();
    return result;
}

void ScheduleConf::setConf(int zone, schedule_t *conf)
{
    store->begin(_nameSpace.c_str(), false);
    store->putBytes(this->_zoneNames[zone].c_str(), conf, sizeof(schedule_t));
    store->end();
}