#ifndef PIO_UNIT_TESTING
#include "main.h"

// IDisplay Display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, TFT_RESET);
Adafruit_ST7735 Display(CS_PIN, DC_PIN, MOSI, CLK, 0); // Display object
Keyboard kbrd;

Menu menu(&Display, &kbrd);

void setup()
{
    Serial.begin(115200);
    Display.initR(INITR_BLACKTAB);
    Display.fillScreen(ST7735_BLACK);
    menu.MenusSetup();
}

void loop()
{
    kbrd.update_buttons();
    if (kbrd.button_Enter.pressed())
    {
        Serial.print("-");
        menu.MenusSetup();
    }
    Serial.print(".");
    delay(200);
}

#endif
