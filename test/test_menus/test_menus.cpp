#include <unity.h>

#include "main.h"

#define TFT_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_ST7735 display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
Keyboard kbrd;
Menu menu(&display, &kbrd);

void testMenus()
{
    menu.MenusSetup();
}

//----------------
void setup()
{
    Serial.begin(115200);

    UNITY_BEGIN();
    display.initR(INITR_BLACKTAB);
    display.fillScreen(ST7735_BLACK);
    RUN_TEST(testMenus);
    display.fillScreen(ST7735_BLACK);
    UNITY_END();
}

void loop()
{
}
