#ifndef PIO_UNIT_TESTING
#include "main.h"

#define DEFAULT_DAYS (SUNDAY | TUESDAY | THURSDAY)
#define DEFAULT_HOUR 23
#define DEFAULT_MIN 40
#define DEFAULT_DURATION 5
#define ONE_SECOND 1000 // 1000 miliseconds

Adafruit_ST7735 display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, -1);
Keyboard kbrd;
ScheduleConf sch;
// Mode menuMode;
Menu menu(&display, &kbrd);
unsigned long lastTime = 0; // to mesure one seccond

void setup()
{
    Serial.begin(115200);

    sch.init();
    schedule_t s_val;
    for (int i = 0; i < MAX_ZONES; i++)
    {
        s_val = sch.getConf(i);
        if (menu.setZoneConf(i, s_val.days, s_val.hour, s_val.minute, s_val.duration, s_val.humidity, s_val.rain, s_val.pir))
        {
            Serial.println("Error loading data to the menus");
        }
    }

    // if menu mode is true, call the menu
    // if (menuMode.getMode())
    if (sch.getMode())
    {
        menu.MenusSetup();
        for (int i = 0; i < MAX_ZONES; i++)
        {
            s_val.days = menu.getZoneConfdays(i);
            s_val.hour = menu.getZoneConfhour(i);
            s_val.minute = menu.getZoneConfmin(i);
            s_val.duration = menu.getZoneConfduration(i); // get it in seconds
            s_val.humidity = menu.getZoneConfHumidity();
            s_val.rain = menu.getZoneConfRain();
            s_val.pir = menu.getZoneConfPir();
            // Serial.printf("-%d - 0x%X %d %d %d %d %d %d\n", i, s_val.days, s_val.hour, s_val.minute, (s_val.duration / 60), s_val.humidity, s_val.rain, s_val.pir);
            sch.setConf(i, &s_val);
            s_val = sch.getConf(i);
        }
        sch.setMode(false);
    }
    // we get here either because the menu was not called or already exit from it
}

void loop()
{

    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
        // come here every second
        kbrd.update_buttons();
        if (kbrd.button_Enter.pressed())
        {
            // press the enter button to call the menu
            menu.MenusSetup();
            schedule_t s;
            for (int i = 0; i < MAX_ZONES; i++)
            {
                s.days = menu.getZoneConfdays(i);
                s.hour = menu.getZoneConfhour(i);
                s.minute = menu.getZoneConfmin(i);
                s.duration = menu.getZoneConfduration(i);
                s.humidity = menu.getZoneConfHumidity();
                s.rain = menu.getZoneConfRain();
                s.pir = menu.getZoneConfPir();
                // save in memory
                sch.setConf(i, &s);
            }
        }
        lastTime = millis();
    }
}

#endif
