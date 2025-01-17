#ifndef PIO_UNIT_TESTING
#include "main.h"

#define DEFAULT_DAYS (SUNDAY | TUESDAY | THURSDAY)
#define DEFAULT_HOUR 23
#define DEFAULT_MIN 40
#define DEFAULT_DURATION 5
#define ONE_SECOND 1000 // 1000 miliseconds

// Adafruit_ST7735 display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, -1);
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
tm currentTime; // to mesure one seccond
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
}

void loop()
{
    // this is required for OTA feature.
    mWiFi.loop();

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
            display.fillScreen(TFT_BLACK);
        }
        if (kbrd.button_Down.pressed())
        {
            mWiFi.show(&iOled);
            iReley.turnOn(1);
        }
        if (kbrd.button_Up.pressed())
        {
            iTime.showOled(&iOled);
            iReley.tunrOFF(1);
        }
        lastTime = millis();
    }
}

#endif
