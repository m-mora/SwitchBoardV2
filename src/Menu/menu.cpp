#include "menu.h"

Menu::Menu(Adafruit_ST7735 *D, Keyboard *b)
{
    // Adafruit_ST7735 Display(CS_PIN, DC_PIN, MOSI, CLK, 0); // Display object
    // Buttons btn;

    Display = D;
    btn = b;

    MainMenu = new ItemMenu(Display);
    ConfigurationMenu = new ItemMenu(Display); // default is false, need not specify

    Zone1Menu = new EditMenu(Display);
    Zone2Menu = new EditMenu(Display);
    Zone3Menu = new EditMenu(Display);
    Zone4Menu = new EditMenu(Display);

    SensorsMenu = new EditMenu(Display);
}

Menu::~Menu()
{
    delete (MainMenu);
    delete (ConfigurationMenu);

    delete (Zone1Menu);
    delete (Zone2Menu);
    delete (Zone3Menu);
    delete (Zone4Menu);

    delete (SensorsMenu);
}

void Menu::ProcessSensors()
{
  // set an inital flag that will be used to store what menu item the user exited on
  int SensorMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  SensorsMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (SensorMenuOption != 0)
  {

    btn->update_buttons();
    if(btn->button_Up.pressed())
    {
        SensorsMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      SensorsMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed()) {
      SensorMenuOption = SensorsMenu->selectRow();
    }
  }

  // out of menu now time for processing

  // set global back color
  Serial.println("Sensors");

  Serial.print("Rain "); Serial.println(SensorsMenu->value[rainSensor]);
  Serial.print("Humidity "); Serial.println(SensorsMenu->value[humiditySensor]);
  Serial.print("PIR ");Serial.println(SensorsMenu->value[pirSensor]);
}

void Menu::ProcessZone(int zone)
{
  // set an inital flag that will be used to store what menu item the user exited on
  int ZoneMenuOption = 1;
  EditMenu *zMenu;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  // draw the main menu
  switch (zone)
  {
  case 0:
    zMenu = Zone1Menu;
    break;
  case 1:
    zMenu = Zone2Menu;
    break;
  case 2:
    zMenu = Zone3Menu;
    break;
  case 3:
    zMenu = Zone4Menu;
    break;
  default:
    break;
  }
  zMenu->draw();
  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (ZoneMenuOption != 0)
  {

    btn->update_buttons();
    if(btn->button_Up.pressed())
    {
        zMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      zMenu->MoveDown();
    }
    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed()) {
      ZoneMenuOption = zMenu->selectRow();
    }
  }

  // out of menu now time for processing

  // set global back color
  Serial.println("Configuration");
  uint8_t week;
  week = ((uint8_t)zMenu->value[ZoneSun[zone]] << 6 |
          (uint8_t)zMenu->value[ZoneMon[zone]] << 5 |
          (uint8_t)zMenu->value[ZoneTue[zone]] << 4 |
          (uint8_t)zMenu->value[ZoneWed[zone]] << 3 |
          (uint8_t)zMenu->value[ZoneThu[zone]] << 2 |
          (uint8_t)zMenu->value[ZoneFri[zone]] << 1 |
          (uint8_t)zMenu->value[ZoneSat[zone]]);
  Serial.printf("Zone %d = 0x%X\n", zone + 1, week);
  Serial.println((int)zMenu->value[hourConf[zone]]);
  Serial.println((int)zMenu->value[minConf[zone]]);
  Serial.println((uint16_t)(zMenu->value[durConf[zone]] * 60));
}

void Menu::ProcessConfMenu()
{
  // set an inital flag that will be used to store what menu item the user exited on
  int ConfMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);

  // draw the main menu
  ConfigurationMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit)
  // and selectes it
  while (ConfMenuOption != 0)
  {

    btn->update_buttons();
    if(btn->button_Up.pressed())
    {
        ConfigurationMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      ConfigurationMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed()) {

      ConfMenuOption = ConfigurationMenu->selectRow();
      if (ConfMenuOption == ConfigurationZone1)
      { // Zone1 item
        ProcessZone(0);
        Display->fillScreen(MENU_BACKGROUND);
        ConfigurationMenu->draw();
      }
      if (ConfMenuOption == ConfigurationZone2)
      { // Zone2 item
        ProcessZone(1);
        Display->fillScreen(MENU_BACKGROUND);
        ConfigurationMenu->draw();
      }
      if (ConfMenuOption == ConfigurationZone3)
      { // Zone3 item
        ProcessZone(2);
        Display->fillScreen(MENU_BACKGROUND);
        ConfigurationMenu->draw();
      }
      if (ConfMenuOption == ConfigurationZone4)
      { // Zone4 item
        ProcessZone(3);
        Display->fillScreen(MENU_BACKGROUND);
        ConfigurationMenu->draw();
      }
      if (ConfMenuOption == ConfigurationSensors)
      { // Sensors item
        ProcessSensors();
        Display->fillScreen(MENU_BACKGROUND);
        ConfigurationMenu->draw();
      }
    }
  }
}

void Menu::ProcessMainMenu()
{

  // set an inital flag that will be used to store what menu item the user exited on
  int MainMenuOption = 1;

  // blank out the screen
  Display->fillScreen(MENU_BACKGROUND);
  // draw the main menu
  MainMenu->draw();

  // run the processing loop until user move selector to title bar (which becomes exit, i.e. 0 return val)
  // and selectes it
  // note menu code can return - 1 for errors so run unitl non zero

  while (MainMenuOption != 0)
  {
    btn->update_buttons();
    if(btn->button_Up.pressed())
    {
        MainMenu->MoveUp();
    }
    // Check down
    if (btn->button_Down.pressed())
    {
      MainMenu->MoveDown();
    }

    // but wait...the user pressed the button on the encoder
    if (btn->button_Enter.pressed())
    {
      MainMenuOption = MainMenu->selectRow();

      // here is where you process accordingly
      // remember on pressing button on title bar 0 is returned and will exit the loop

      if (MainMenuOption == MenuOptionAuto)
      {
        return;
      }
      if (MainMenuOption == MenuOptionOnOff)
      {
        if (MainMenu->getEnableState(MenuOptionAuto))
        {
          MainMenu->disable(MenuOptionAuto);
          MainMenu->disable(MenuOptionConf);
        }
        else
        {
          MainMenu->enable(MenuOptionAuto);
          MainMenu->enable(MenuOptionConf);
        }
        MainMenu->draw();
      }
      // Configuration menu was selected
      if (MainMenuOption == MenuOptionConf)
      {

        ProcessConfMenu();
        Display->fillScreen(MENU_BACKGROUND);
        MainMenu->draw();
      }

    }
  }

}

void Menu::MenusSetup()
{

  // Display->begin();
  Display->initR(INITR_BLACKTAB);
  Display->fillScreen(ST7735_BLACK);
  Display->setRotation(0);
  Display->setTextSize(1);
  /*
    init(TextColor, BackgroundColor, HighLtTextColor, HighLtColor,
    ItemROW_HEIGHT, MaxRowsPerScreen, TitleText);
  */
  MainMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, ROW_HEIGHT, MAXROWSPERSCREEN, "Main Menu");
  // MainMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Main Menu");

  // now add each item, text is what's displayed for each menu item, there are no other arguements
  // there are 3 add types
  // 1. addNI for no icon
  // 2. addMono for simple 1 color  icon each item
  // 3. add565 for displaying a color icon for each item
  MenuOptionAuto = MainMenu->addNI("Auto");
  MenuOptionOnOff = MainMenu->addNI("On/Off");
  MenuOptionConf = MainMenu->addNI("Configuration");

  // setTitleColors(TitleTextColor, TitleFillColor);
  MainMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);

  // setTitleBarSize(TitleTop, TitleLeft, TitleWith, TitleHeight);
  MainMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);

  // setTitleTextMargins(LeftMargin, TopMargin);
  MainMenu->setTitleTextMargins(TITLE_LEFT_MARGIN, TITLE_TOP_MARGIN);

  // the library will not determine font height and such and adjust--that will be on you
  // setMenuBarMargins(LeftMargin, Width, BorderRadius, BorderThickness);
  MainMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);

  // the library will not determine font height and such and adjust--that will be on you
  // setItemColors(DisableTextColor, BorderColor);
  MainMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER);

  // setItemTextMargins(LeftMargin, TopMargin, MarginBetweenTitleBarAndMenu);
  MainMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);

  // end of MainMenu setup

  // ConfigurationMenu.init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT,
  //                        DATA_COLUMN, ROW_HEIGHT, ROWS, "Configuration");

  ConfigurationMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, ROW_HEIGHT, MAXROWSPERSCREEN, "Configuration");

  ConfigurationZone1 = ConfigurationMenu->addNI("Zone1");
  ConfigurationZone2 = ConfigurationMenu->addNI("Zone2");
  ConfigurationZone3 = ConfigurationMenu->addNI("Zone3");
  ConfigurationZone4 = ConfigurationMenu->addNI("Zone4");
  ConfigurationSensors = ConfigurationMenu->addNI("Sensors");

  ConfigurationMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  // setTitleColors(TitleTextColor, TitleFillColor);
  ConfigurationMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);

  // setTitleBarSize(TitleTop, TitleLeft, TitleWith, TitleHeight);
  ConfigurationMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);

  // setTitleTextMargins(LeftMargin, TopMargin);
  ConfigurationMenu->setTitleTextMargins((Display->width() - (sizeof("Configuration") * 6)) / 2, TITLE_TOP_MARGIN);

  // setItemTextMargins(LeftMargin, TopMargin, MarginBetweenTitleBarAndMenu);
  ConfigurationMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);

  // setItemColors(DisableTextColor, HighLt, EditSelectBorderColor);
  // ConfigurationMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);
  ConfigurationMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER);

  //------------------------- Zone 1 ----------------------------------------------------
  Zone1Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 1");
  ZoneSun[0] = Zone1Menu->addNI("Sunday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[0] = Zone1Menu->addNI("Monday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[0] = Zone1Menu->addNI("Tuesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[0] = Zone1Menu->addNI("Wednesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[0] = Zone1Menu->addNI("Thursday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[0] = Zone1Menu->addNI("Friday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[0] = Zone1Menu->addNI("Saturday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[0] = Zone1Menu->addNI("Hour", hour[0], 0, 23, 1, 0, NULL);
  minConf[0] = Zone1Menu->addNI("Minute", minute[0], 0, 59, 1, 0, NULL);
  durConf[0] = Zone1Menu->addNI("Duration", duration[0], 0, 59, 1, 0, NULL);

  Zone1Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone1Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone1Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone1Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 1") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone1Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone1Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 2---------------------------------------------------
  Zone2Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 2");
  ZoneSun[1] = Zone2Menu->addNI("Sunday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[1] = Zone2Menu->addNI("Monday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[1] = Zone2Menu->addNI("Tuesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[1] = Zone2Menu->addNI("Wednesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[1] = Zone2Menu->addNI("Thursday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[1] = Zone2Menu->addNI("Friday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[1] = Zone2Menu->addNI("Saturday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[1] = Zone2Menu->addNI("Hour", hour[1], 0, 23, 1, 0, NULL);
  minConf[1] = Zone2Menu->addNI("Minute", minute[1], 0, 59, 1, 0, NULL);
  durConf[1] = Zone2Menu->addNI("Duration", duration[1], 0, 59, 1, 0, NULL);

  Zone2Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone2Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone2Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone2Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 2") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone2Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone2Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 3 ---------------------------------------------------
  Zone3Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 3");
  ZoneSun[2] = Zone3Menu->addNI("Sunday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[2] = Zone3Menu->addNI("Monday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[2] = Zone3Menu->addNI("Tuesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[2] = Zone3Menu->addNI("Wednesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[2] = Zone3Menu->addNI("Thursday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[2] = Zone3Menu->addNI("Friday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[2] = Zone3Menu->addNI("Saturday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[2] = Zone3Menu->addNI("Hour", hour[2], 0, 23, 1, 0, NULL);
  minConf[2] = Zone3Menu->addNI("Minute", minute[2], 0, 59, 1, 0, NULL);
  durConf[2] = Zone3Menu->addNI("Duration", duration[2], 0, 59, 1, 0, NULL);

  Zone3Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone3Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone3Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone3Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 3") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone3Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone3Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Zone 4 ---------------------------------------------------
  Zone4Menu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN, ROW_HEIGHT, ROWS, "Zone 4");
  ZoneSun[3] = Zone4Menu->addNI("Sunday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneMon[3] = Zone4Menu->addNI("Monday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneTue[3] = Zone4Menu->addNI("Tuesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneWed[3] = Zone4Menu->addNI("Wednesday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneThu[3] = Zone4Menu->addNI("Thursday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneFri[3] = Zone4Menu->addNI("Friday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  ZoneSat[3] = Zone4Menu->addNI("Saturday", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);

  hourConf[3] = Zone4Menu->addNI("Hour", hour[3], 0, 23, 1, 0, NULL);
  minConf[3] = Zone4Menu->addNI("Minute", minute[3], 0, 59, 1, 0, NULL);
  durConf[3] = Zone4Menu->addNI("Duration", duration[3], 0, 59, 1, 0, NULL);

  Zone4Menu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  Zone4Menu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  Zone4Menu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  Zone4Menu->setTitleTextMargins((Display->width() - (sizeof("Zone 4") * 6)) / 2, TITLE_TOP_MARGIN);
  Zone4Menu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  Zone4Menu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-------------------------- Sensors  ---------------------------------------------------
  SensorsMenu->init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT, DATA_COLUMN - 20, ROW_HEIGHT, ROWS, "Sensors");
  humiditySensor = SensorsMenu->addNI("Humidity", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  rainSensor = SensorsMenu->addNI("Rain", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems);
  pirSensor = SensorsMenu->addNI("Precense", 0, 0, sizeof(presenceSensor) / sizeof(presenceSensor[0]), 1, 0, presenceSensor);

  SensorsMenu->setMenuBarMargins(0, Display->width(), BORDERRADIUS, BORDERTHICKNESS);
  SensorsMenu->setTitleColors(TITLE_TEXT, TITLE_BACK);
  SensorsMenu->setTitleBarSize(0, 0, Display->width(), TITLE_HEIGHT);
  SensorsMenu->setTitleTextMargins((Display->width() - (sizeof("Sensors") * 6)) / 2, TITLE_TOP_MARGIN);
  SensorsMenu->setItemTextMargins(LEFTMARGIN, TOPMARGIN, MARGINBETWEENTITLEBARANDMENU);
  SensorsMenu->setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER);

  //-----------------------------------------------------------------------------

  ProcessMainMenu();

  // menu code done, now proceed to your code
  Display->fillScreen(MENU_TEXT); // Black
}
