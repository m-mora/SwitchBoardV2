#include <unity.h>

#include "main.h"

IDisplay *display;
Keyboard kbrd;

bool prev_state[MAX_BUTTONS] = {1, 1, 1, 1, 1};
const uint8_t rowShift = 9;
String selectedButton[MAX_BUTTONS] = {"Up", "Down", "Left", "Right", "Enter"};
uint64_t timeSincePressed[MAX_BUTTONS];

void update_display_message_by_row(uint8_t row, bool state)
{
    uint16_t x_pos = display->width() - 53;
    uint16_t y_pos = row * rowShift;
    if (state)
    {
        display->setTextSize(1);
        display->setCursor(0, y_pos);
        display->setTextColor(IDISPLAY_BLACK);
        display->fillRect(0, y_pos, display->width(), 8, IDISPLAY_BLACK);
        display->setCursor(0, y_pos);
        display->setTextColor(IDISPLAY_BLUE);
        display->print("Button ");
        display->setTextColor(IDISPLAY_CYAN);
        display->print(selectedButton[row]);
        display->setCursor(x_pos, y_pos);
        display->setTextColor(IDISPLAY_RED);
        display->print("pressed");
        display->display();
        Serial.printf("Button %s pressed \n", selectedButton[row]);
        prev_state[row] = 1;
        timeSincePressed[row] = millis();
    }
    else if (prev_state[row] && (millis() - timeSincePressed[row]) > 1000)
    {
        display->setTextSize(1);
        display->setCursor(0, y_pos);
        display->setTextColor(IDISPLAY_BLACK);
        display->fillRect(0, y_pos, display->width(), 8, IDISPLAY_BLACK);
        display->setCursor(0, y_pos);
        display->setTextColor(IDISPLAY_BLUE);
        display->print("Button ");
        display->setTextColor(IDISPLAY_CYAN);
        display->print(selectedButton[row]);
        display->setCursor(x_pos, y_pos);
        display->setTextColor(IDISPLAY_GREEN);
        display->print("released");
        display->display();
        Serial.printf("Button %s released \n", selectedButton[row]);
        prev_state[row] = 0;
    }
    display->display();
}

void test_button_press()
{
    kbrd.update_buttons();

    update_display_message_by_row(UP, kbrd.button_Up.pressed());
    update_display_message_by_row(DOWN, kbrd.button_Down.pressed());
    update_display_message_by_row(LEFT, kbrd.button_Left.pressed());
    update_display_message_by_row(RIGHT, kbrd.button_Right.pressed());
    update_display_message_by_row(ENTER, kbrd.button_Enter.pressed());

    delay(100);
}

//----------------
void setup()
{
    Serial.begin(115200);
    display = new ST7735Display(SPI_CS, SPI_DC, SPI_MOSI, SPI_SCK, -1);
    display->init();
    display->clearScreen();
    display->display();

    UNITY_BEGIN();
}

void loop()
{
    RUN_TEST(test_button_press);

    UNITY_END();
}
