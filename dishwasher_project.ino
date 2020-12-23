#include <Arduino.h>
#include <EasyButton.h>

#include "Lcd.h"
#include "enums.h"

//const BTN_PRESSED = 0; // inverse because of pullup input
//const BTN_RELEASED = 1; // inverse because of pullup input

const int BTN_PIN_1 = 14; // Start/Fwd button. GPIO 14 (D5)
const int BTN_PIN_2 = 12; // Stop/Back button. GPIO 12 (D6)

EasyButton btn1(BTN_PIN_1);
EasyButton btn2(BTN_PIN_2);

//int button1PrevState = BTN_RELEASED;
//int button2PrevState = BTN_RELEASED;

mode CURRENT_MODE = MODE_HOT;
duration CURRENT_DURATION = DURATION_QUICK;
state CURRENT_STATE = STATE_WELCOME;

LCD_Controller lcd; // Initialize LCD

void onBtn1Pressed() {
    Serial.println("Button1 has been pressed!");
    switch (CURRENT_STATE) {
        case STATE_WELCOME:
            CURRENT_STATE = STATE_MOD_SELECT;
        case STATE_MOD_SELECT:
            CURRENT_MODE = nextMode(CURRENT_MODE);
            lcd.showMode(CURRENT_MODE);
            break;
        case STATE_DURATION_SELECT:
            CURRENT_DURATION = nextDuration(CURRENT_DURATION);
            lcd.showDuration(CURRENT_DURATION);
            break;
        case STATE_FUNCTIONAL:
            break;
    }
}

void onBtn1LongPressed() {
    Serial.println("Button1 has been long pressed!");
    switch (CURRENT_STATE) {
        case STATE_MOD_SELECT:
            CURRENT_STATE = STATE_DURATION_SELECT;
            lcd.showDuration(CURRENT_DURATION);
            break;
        case STATE_DURATION_SELECT:
            break;
    }
}

void onBtn2Pressed() {
    Serial.println("Button2 has been pressed!");
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(100);
    Serial.println(F("Initializing dishwasher program..."));
    lcd.init();
    lcd.splash();

    btn1.begin();
    btn1.onPressed(onBtn1Pressed);
    btn1.onPressedFor(2000, onBtn1LongPressed);
    btn2.begin();
    btn2.onPressed(onBtn2Pressed);

    Serial.println(F("... Setup completed"));
}

void loop() {
    btn1.read();
    btn2.read();

}


void loopSpeedControl() {
    delay(100);
}