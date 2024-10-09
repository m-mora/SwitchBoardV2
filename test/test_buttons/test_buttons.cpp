#include <unity.h>

#include "main.h"

#define MAXBUTTONS  5

#define LEDPINUP 18
#define LEDPINDOWN 19
#define LEDPINRIGHT 20
#define LEDPINLEFT 21
#define LEDPINENTER 22

Buttons buttonUp(BUTTONUP, "ButtonUp");
Buttons buttonDown(BUTTONDOWN, "ButtonDown");
Buttons buttonRight(BUTTONRIGHT, "ButtonRight");
Buttons buttonLeft(BUTTONLEFT, "ButtonLeft");
Buttons buttonEnter(BUTTONENTER, "ButtonEnter");
Buttons button[] = {buttonUp, buttonDown, buttonRight, buttonLeft, buttonEnter};

void test_buttons()
{
    buttonUp.update();
    buttonDown.update();
    buttonLeft.update();
    buttonRight.update();
    buttonEnter.update();

    if (buttonUp.state())
    {
        digitalWrite(LEDPINUP, buttonUp.state());
    }
    digitalWrite(LEDPINDOWN, buttonDown.state());
    digitalWrite(LEDPINRIGHT, buttonRight.state());
    digitalWrite(LEDPINLEFT, buttonLeft.state());
    digitalWrite(LEDPINENTER, buttonEnter.state());
}

void test_button_initial_low()
{
    TEST_ASSERT_EQUAL(HIGH, digitalRead(BUTTONUP));
}

//----------------
void setup()
{
    Serial.begin(115200);
    UNITY_BEGIN();
    for (int index = 0; index < MAXBUTTONS; index++)
    {
        button[index].init();
    }
    RUN_TEST(test_button_initial_low);

    pinMode(LEDPINUP, OUTPUT);
    pinMode(LEDPINDOWN, OUTPUT);
    pinMode(LEDPINLEFT, OUTPUT);
    pinMode(LEDPINRIGHT, OUTPUT);
    pinMode(LEDPINENTER, OUTPUT);}

void loop()
{
    RUN_TEST(test_buttons);
    UNITY_END();
}
