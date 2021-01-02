#include <Arduino.h>
#include <EasyButton.h>

#include "Lcd.h"
#include "enums.h"
//#include "Buzzer.h"

//const BTN_PRESSED = 0; // inverse because of pullup input
//const BTN_RELEASED = 1; // inverse because of pullup input

const int BTN_PIN_1 = 1; // Start/Fwd button. GPIO 1 (TX)
const int BTN_PIN_2 = 2; // Stop/Back button. GPIO 2 (D4)
EasyButton btn1(BTN_PIN_1);
EasyButton btn2(BTN_PIN_2);

const int BUZZER_PIN = 12; // GPIO 12 (D6)
//Buzzer_Controller buzzer;

//int button1PrevState = BTN_RELEASED;
//int button2PrevState = BTN_RELEASED;

dw_mode CURRENT_MODE = MODE_HOT;
dw_duration CURRENT_DURATION = DURATION_QUICK;
state CURRENT_STATE = STATE_WELCOME;

LCD_Controller lcd; // Initialize LCD

void onBtn1Pressed() {
    Serial.println("Button1 has been pressed!");
    switch (CURRENT_STATE) {
        case STATE_WELCOME:
            CURRENT_STATE = STATE_MOD_SELECT;
        case STATE_MOD_SELECT:
            CURRENT_MODE = nextDwMode(CURRENT_MODE);
            lcd.showMode(CURRENT_MODE);
            break;
        case STATE_DURATION_SELECT:
            CURRENT_DURATION = nextDwDuration(CURRENT_DURATION);
            lcd.showDuration(CURRENT_DURATION);
            break;
        case STATE_FUNCTIONAL:
            break;
    }
//    buzzer.singleBeep(100);
}

void onBtn1LongPressed() {
    Serial.println("Button1 has been long pressed!");
    switch (CURRENT_STATE) {
        case STATE_MOD_SELECT:
            CURRENT_STATE = STATE_DURATION_SELECT;
            lcd.showDuration(CURRENT_DURATION);
            break;
        case STATE_DURATION_SELECT:
            CURRENT_STATE = STATE_CONFIRMATION;
            lcd.showConfirmation(CURRENT_MODE, CURRENT_DURATION);
            break;
        case STATE_CONFIRMATION:
            CURRENT_STATE = STATE_CONFIRMATION;
            lcd.showFunctionalDetails();
            break;
    }
}

void onBtn2Pressed() {
    Serial.println("Button2 has been pressed!");
    // Stop all functions
    CURRENT_STATE = STATE_WELCOME;
    CURRENT_MODE = MODE_HOT;
    CURRENT_DURATION = DURATION_QUICK;
    lcd.splash();
//    buzzer.singleBeep(100);
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(100);
    Serial.println(F("Initializing dishwasher program..."));
    lcd.init();
    lcd.splash();

    btn1.begin();
    btn1.onPressed(onBtn1Pressed);
    btn1.onPressedFor(1000, onBtn1LongPressed);
    btn2.begin();
    btn2.onPressed(onBtn2Pressed);

//    buzzer.init(BUZZER_PIN);

    Serial.println(F("... Setup completed"));
//    buzzer.multipleBeep(100, 5);
}

void loop() {
    btn1.read();
    btn2.read();
//    buzzer.update();

    if (CURRENT_STATE == STATE_FUNCTIONAL) { // If dishwasher is running

    }

}

