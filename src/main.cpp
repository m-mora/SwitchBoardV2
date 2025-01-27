#ifndef PIO_UNIT_TESTING
#include "main.h"

#define DEFAULT_DAYS (SUNDAY | TUESDAY | THURSDAY)
#define DEFAULT_HOUR 23
#define DEFAULT_MIN 40
#define DEFAULT_DURATION 5
#define ONE_SECOND 1000 // 1000 miliseconds
#define MANUAL_CONTROL_ENABLE 0x8000

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
uint16_t manualC = 0x0000;
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

void clearOledDisplay()
{
    iOled.clearDisplay();
    iOled.display();
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
        Serial.printf("SAVED Zone %d: %02d:%02d\n", i + 1, currConf[i].hour, currConf[i].minute);
    }
}

void showReleyStatusInDisplay(uint8_t zone, uint8_t hour, uint8_t min, uint8_t duration, bool show)
{
    if (show)
    {
        iOled.clearDisplay();
        iOled.setCursor(10, 0);
        iOled.setTextSize(1);
        iOled.setTextColor(SSD1306_WHITE);
        iOled.printf("Zone %d is ON\n\n", zone + 1);
        iOled.printf("Time: %02d:%02d\n", hour, min);
        iOled.printf("Duration: %d min\n", duration / 60);
        iOled.display();
    }
    else
    {
        clearOledDisplay();
    }
}

void programedControl()
{
    tm t;
    t = iTime.getTimeDate();

    for (int i = 0; i < MAX_ZONES; i++)
    {
        if (iReley.isOn(i))
        {
            // The reley is on, check if needs to turn it off
            if (iReley.timeOninSeconds(i) >= currConf[i].duration)
            {
                iReley.turnOff(i);
                showReleyStatusInDisplay(i, currConf[i].hour, currConf[i].minute, currConf[i].duration, false);
                Serial.printf("Zone %d OFF: ", i);
                Serial.printf("%d, %d %02d:%02d:%02d", t.tm_wday, t.tm_mon, t.tm_hour, t.tm_min, t.tm_sec);
            }
        }
        else
        {
            // if true means that this day needs to turn on the correspondig zone
            if (((1 << 6 - t.tm_wday) & currConf[i].days) && // in tm structure sunday is 0, in the conf, bit 6 is sunday
                (t.tm_hour == currConf[i].hour) &&
                (t.tm_min == currConf[i].minute))
            {
                iReley.turnOn(i);
                showReleyStatusInDisplay(i, currConf[i].hour, currConf[i].minute, currConf[i].duration, true);
                Serial.printf("Zone %d  ON: ", i);
                // Serial.println(conf, "%A, %B %d %Y %H:%M:%S");
                Serial.printf("%d, %d %02d:%02d:%02d", t.tm_wday, t.tm_mon, t.tm_hour, t.tm_min, t.tm_sec);
            }
        }
    }
}

void manualControl()
{
    if (manualC)
    {
        // an action was requested from telegram
        switch (manualC & 0x7FFF)
        {
        case ZONE1ON:
            iReley.turnOn(ZONE1);
            break;
        case ZONE2ON:
            iReley.turnOn(ZONE2);
            break;
        case ZONE3ON:
            iReley.turnOn(ZONE3);
            break;
        case ZONE4ON:
            iReley.turnOn(ZONE4);
            break;
        case ZONE1OFF:
            iReley.turnOff(ZONE1);
            break;
        case ZONE2OFF:
            iReley.turnOff(ZONE2);
            break;
        case ZONE3OFF:
            iReley.turnOff(ZONE3);
            break;
        case ZONE4OFF:
            iReley.turnOff(ZONE4);
            break;
        case MSG_EXIT:
            // ensure all zones are turned off
            for(int i = 0; i < MAX_ZONES; i++)
            {
                iReley.turnOff(i);
            }
            break;
        default:
            break;
        }
    }
}

void checkIfActionIsRequired()
{
    if (manualC & MANUAL_CONTROL_ENABLE)
    {
        manualControl();
    }
    else
    {
        programedControl();
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
    delay(5000);
    clearOledDisplay();
}

void loop()
{
    // this is required for OTA feature.
    manualC = mWiFi.loop();

    /* Enter in this if every second*/
    if (millis() - lastTime >= ONE_SECOND)
    {
        checkIfActionIsRequired();
        if (fiveSeconds == 5)
        {
            // iOled.clearDisplay();
            // iOled.display();
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
            iReley.turnOn(1);
        }
        if (kbrd.button_Up.pressed())
        {
            fiveSeconds = 0;
            iTime.showOled(&iOled);
            iReley.turnOff(1);
        }
        if (kbrd.button_Left.pressed())
        {
            clearOledDisplay();
        }

        if (kbrd.button_Right.pressed())
        {
            mWiFi.show(&iOled);
        }
        lastTime = millis();
    }
}

#endif
