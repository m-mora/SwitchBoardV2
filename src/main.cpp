#ifndef PIO_UNIT_TESTING
#include "main.h"

#define DEFAULT_DAYS (SUNDAY | TUESDAY | THURSDAY)
#define DEFAULT_HOUR 23
#define DEFAULT_MIN 40
#define DEFAULT_DURATION 5
#define ONE_SECOND 1000 // 1000 miliseconds

TFT_eSPI display = TFT_eSPI();
Adafruit_SSD1306 iOled(128, 64, &Wire, -1);

// instance objects
Keyboard kbrd;
ScheduleConf sch;
MyWifi mWiFi;
ITime iTime;
Reley iReley;
Menu menu(&display, &kbrd);

// global Varibles
unsigned long lastTime = 0;
tm currentTime;
schedule_t currConf[4];
uint8_t fiveSeconds = 0;

void mainInitialization()
{
    mWiFi.init();
    ScanI2CDevicesAndDumpTable();
    iOled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    iOled.clearDisplay();
    iOled.display();
    display.begin();
    display.fillScreen(TFT_BLACK);
    mWiFi.show(&iOled);
    iTime.init();
    iReley.init();
    sch.init();
}

void updateTimeInMenu()
{
    if (iTime.isValid())
    {
        currentTime = iTime.getTimeDate();
    }
    else
    {
        // if not valid time provider, set a default
        currentTime.tm_year = 2025 - 1900; // year is saved relative to 1900
        currentTime.tm_mday = 19;
        currentTime.tm_mon = 1;
        currentTime.tm_hour = 22;
        currentTime.tm_min = 20;
        currentTime.tm_sec = 50;
    }
    menu.setTime(currentTime);
}

void updateZoneConfiguration()
{
    schedule_t conf;
    for (int i = 0; i < MAX_ZONES; i++)
    {
        conf = sch.getConf(i);
        currConf[i] = conf;
    }
}

void showReleyStatusInDisplay(uint8_t zone, uint8_t hour, uint8_t min, uint8_t duration)
{
    iOled.clearDisplay();
    iOled.setCursor(10,0);
    iOled.setTextSize(1);
    iOled.setTextColor(SSD1306_WHITE);
    iOled.printf("Zone %d  is ON\n\n",zone + 1);
    iOled.printf("Hour: %d\n",hour);
    iOled.printf("Minute: %d\n",min);
    iOled.printf("Duration: %d\n",duration);
    iOled.display();
}

void checkIfActionIsRequired()
{
    tm conf;
    conf = iTime.getTimeDate();

    for (int i = 0; i < MAX_ZONES; i++)
    {
        if (iReley.isOn(i))
        {
            // The reley is on, check if needs to turn it off
            if (iReley.timeOninSeconds(i) >= currConf[i].duration)
            {
                iReley.tunrOFF(i);
                Serial.printf("Zone %d OFF: ", i);
                Serial.printf("%d, %d %d:%d:%d", conf.tm_wday, conf.tm_mon, conf.tm_hour, conf.tm_min, conf.tm_sec);
            }
        }
        else
        {
            // if true means that this day needs to turn on the correspondig zone
            if (((6 - conf.tm_wday) & currConf[i].days) &&
                (conf.tm_hour == currConf[i].hour) &&
                (conf.tm_min == currConf[i].minute))
            {
                iReley.turnOn(i);
                showReleyStatusInDisplay(i,currConf[i].hour,currConf[i].minute,currConf[i].duration);
                Serial.printf("Zone %d  ON: ", i);
                // Serial.println(conf, "%A, %B %d %Y %H:%M:%S");
                Serial.printf("%d, %d %d:%d:%d", conf.tm_wday, conf.tm_mon, conf.tm_hour, conf.tm_min, conf.tm_sec);
            }
        }
    }
}


void setup()
{
    Serial.begin(115200);

    mainInitialization();

    schedule_t s_val;
    for (int i = 0; i < MAX_ZONES; i++)
    {
        // if there is configuration saved, load the values in the GUI menu
        s_val = sch.getConf(i);

        if (menu.setZoneConf(i, s_val.days, s_val.hour, s_val.minute,
                             s_val.duration, s_val.humidity, s_val.rain,
                             s_val.pir))
        {
            Serial.println("Error loading data to the menus");
        }
    }
    // if there is a time provider (NTP or RTC set the time in the menu)
    updateTimeInMenu();
    // copy the configuratin for each zone in a local variable.
    updateZoneConfiguration();
}

void loop()
{
    // this is required for OTA feature.
    mWiFi.loop();
    checkIfActionIsRequired();

    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
        if (fiveSeconds == 5)
        {
            iOled.clearDisplay();
            iOled.display();
            fiveSeconds = 0;
        }
        else
        {
            fiveSeconds++;
        }
        // Check if a button was pressed
        kbrd.update_buttons();

        if (kbrd.button_Enter.pressed())
        {
            // Enter button pressed
            Serial.println("Enter button pressed");
            updateTimeInMenu();
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
            // update configuratin for each zone locally in case it was modified.
            updateZoneConfiguration();
            display.fillScreen(TFT_BLACK);
        }
        if (kbrd.button_Down.pressed())
        {
            fiveSeconds = 0;
            mWiFi.show(&iOled);
            iReley.turnOn(1);
        }
        if (kbrd.button_Up.pressed())
        {
            fiveSeconds = 0;
            iTime.showOled(&iOled);
            iReley.tunrOFF(1);
        }
        lastTime = millis();
    }
}

#endif
