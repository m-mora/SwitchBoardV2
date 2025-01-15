#ifndef PIO_UNIT_TESTING
#include "main.h"

#define DEFAULT_DAYS (SUNDAY | TUESDAY | THURSDAY)
#define DEFAULT_HOUR 23
#define DEFAULT_MIN 40
#define DEFAULT_DURATION 5
#define ONE_SECOND 1000 // 1000 miliseconds

// Adafruit_ST7735 display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, -1);
TFT_eSPI display = TFT_eSPI();
Adafruit_SSD1306 dOled(128, 64, &Wire, -1);

Keyboard kbrd;
ScheduleConf sch;
MyWifi mWiFi;
ntp ntpTime;
Rtc rtc;

Menu menu(&display, &kbrd);
unsigned long lastTime = 0;
tm currentTime; // to mesure one seccond

void print_time_oled(tm* c);

void setup()
{
    Serial.begin(115200);
    // WiFiInit();   // initilize wifi connection and OTA service
    mWiFi.init();
    ScanI2CDevicesAndDumpTable();
    dOled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    dOled.clearDisplay();
    mWiFi.show(&dOled);
    ntpTime.init();
    rtc.init();

    sch.init();
    schedule_t s_val;
    for (int i = 0; i < MAX_ZONES; i++)
    {
        s_val = sch.getConf(i);

        if (menu.setZoneConf(i, s_val.days, s_val.hour, s_val.minute,
                             s_val.duration, s_val.humidity, s_val.rain,
                             s_val.pir))
        {
            Serial.println("Error loading data to the menus");
        }
    }
    display.fillScreen(TFT_BLACK);

    if(ntpTime.getTime(&currentTime))
    {
        // NTP is working, update the RTC
        rtc.set(currentTime.tm_sec, currentTime.tm_min, currentTime.tm_hour,currentTime.tm_mday,currentTime.tm_mon+1,currentTime.tm_year+1900);
        rtc.printTime();
    }
}

uint8_t fiveSeconds = 0;
void loop()
{
    // OTAloop();
    mWiFi.loop();

    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
        if (fiveSeconds == 5)
        {
            dOled.clearDisplay();
            dOled.display();
            fiveSeconds = 0;
        }
        else
        {
            fiveSeconds++;
        }

        // come here every second
        kbrd.update_buttons();
        if (kbrd.button_Enter.pressed())
        {
            Serial.println("Enter button pressed");
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
        if (kbrd.button_Down.pressed())
        {
            mWiFi.show(&dOled);
        }
        if (kbrd.button_Up.pressed())
        {
            ntpTime.getTime(&currentTime);
            ntpTime.print_time_oled(&dOled,&currentTime,true,false);
            rtc.printInOled(&dOled,false);

        }
        display.fillScreen(TFT_BLACK);
        lastTime = millis();
    }
}



#endif
