#pragma once

#include <Arduino.h>
// required libraries
#include "menu_manager.h"
#include "../buttons/keyboard.h"
#include <Adafruit_ST7735.h>
//#include "../Display/IDisplay.h"

// pins select button on encoder
#define CS_PIN 5
#define DC_PIN 19
#define UP_PIN 35
#define DN_PIN 33
#define EN_PIN 14
#define MOSI 23
#define CLK 18

#define ROW_HEIGHT 20
#define DATA_COLUMN 100
#define TITLE_HEIGHT 20
#define MAXROWSPERSCREEN ((160 - TITLE_HEIGHT) / ROW_HEIGHT) // ST7735 is 128x160
#define ROWS MAXROWSPERSCREEN
#define TITLE_LEFT_MARGIN 40
#define TITLE_TOP_MARGIN 6
#define BORDERRADIUS 4
#define BORDERTHICKNESS 2
#define LEFTMARGIN 10
#define TOPMARGIN 5
#define MARGINBETWEENTITLEBARANDMENU 4

// set default colors
#define MENU_TEXT 0x0000          // Black
#define MENU_BACKGROUND 0xFFFF    // White
#define MENU_HIGHLIGHTTEXT 0x0000 // Black
#define MENU_HIGHLIGHT 0x157c     //
#define MENU_HIGHBORDER 0x157D    //
#define MENU_SELECTTEXT 0x0000    // Black
#define MENU_SELECT 0x07E0        // Green
#define MENU_SELECTBORDER 0x0620  // darker green
#define MENU_DISABLE 0xC618       // Grey
#define TITLE_TEXT 0xE71C         // light grey
#define TITLE_BACK 0x0812         // some blue

class Menu
{
private:
    // must have variables for each menu item
    // best to have these global so you can use them in processing functions
    int MenuOptionAuto = 0;
    int MenuOptionOnOff = 0;
    int MenuOptionConf = 0;

    int ConfigurationZone1 = 0;
    int ConfigurationZone2 = 0;
    int ConfigurationZone3 = 0;
    int ConfigurationZone4 = 0;
    int ConfigurationSensors = 0;
    int humiditySensor = 0;
    int rainSensor = 0;
    int pirSensor = 0;

    uint8_t days[4]{0x7F, 0x7F, 0x7F, 0x7F};
    uint8_t hour[4] = {23, 23, 23, 23};
    uint8_t minute[4] = {10, 20, 30, 40};
    uint16_t duration[4] = {5, 5, 5, 5};

    int hourConf[4] = {0, 0, 0, 0};
    int minConf[4] = {0, 0, 0, 0};
    uint16_t durConf[4] = {0, 0, 0, 0};
    int ZoneSun[4] = {0, 0, 0, 0};
    int ZoneMon[4] = {0, 0, 0, 0};
    int ZoneTue[4] = {0, 0, 0, 0};
    int ZoneWed[4] = {0, 0, 0, 0};
    int ZoneThu[4] = {0, 0, 0, 0};
    int ZoneFri[4] = {0, 0, 0, 0};
    int ZoneSat[4] = {0, 0, 0, 0};

    // create some selectable menu sub-items, these are lists inside a menu item
    const char *OffOnItems[2] = {"Off", "On"};
    const char *presenceSensor[3] = {"Off", "Active", "Stop"};

    Adafruit_ST7735 *Display;
    Keyboard *btn;

    ItemMenu *MainMenu;

    // EditMenu ConfigurationMenu(&Display); // default is false, need not specify
    ItemMenu *ConfigurationMenu; // default is false, need not specify

    EditMenu *Zone1Menu;
    EditMenu *Zone2Menu;
    EditMenu *Zone3Menu;
    EditMenu *Zone4Menu;

    EditMenu *SensorsMenu;

public:
    Menu(Adafruit_ST7735 *Display, Keyboard *btn);
    ~Menu();
    void MenusSetup();
    void ProcessMainMenu();
    void ProcessConfMenu();
    void ProcessZone(int zone);
    void ProcessSensors();
};
