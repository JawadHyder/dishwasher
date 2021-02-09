#include <Arduino.h>
#include <EasyButton.h>
#include <Ticker.h>

#include "Lcd.h"
#include "enums.h"
#include "Buzzer.h"
#include "Relay.h"

const int BTN_PIN_1 = 3; // Start/Fwd button. GPIO 3 (RX)
const int BTN_PIN_2 = 2; // Stop/Back button. GPIO 2 (D4)
EasyButton btn1(BTN_PIN_1);
EasyButton btn2(BTN_PIN_2);

const int BUZZER_PIN = 12; // GPIO 12 (D6)
Buzzer_Controller buzzer;

const uint8_t SPRAY_MOTOR_PIN = 14; // GPIO 14 (D5)
Relay_Controller sprayMotor;
const uint8_t DRAIN_MOTOR_PIN = 13; // GPIO 13 (D7)
Relay_Controller drainMotor;
const uint8_t HEATER_PIN = 0; // GPIO 15 (D6)
Relay_Controller heater;

const uint8_t LOW_WATER_LEVEL_PIN = 9; // GPIO 15 (D6)


dw_mode CURRENT_MODE;
dw_duration CURRENT_DURATION;
dw_state CURRENT_STATE;
dw_cycle CURRENT_CYCLE;

LCD_Controller lcd; // Initialize LCD

// Ticks every second when program is in functional state
Ticker operationTicker;
Ticker resetTicker;

void initializeVars() {
    CURRENT_MODE = MODE_HOT;
    CURRENT_DURATION = DURATION_QUICK;
    CURRENT_STATE = STATE_WELCOME;
    CURRENT_CYCLE = CYCLE_1;
}

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
    buzzer.beep(50);
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
            CURRENT_STATE = STATE_FUNCTIONAL;
            lcd.showFunctionalDetails();
            operationTicker.attach(1, operationTick);
            break;
    }
    buzzer.beep(200);
}

void onBtn2Pressed() {
    Serial.println("Button2 has been pressed!");
    // Stop all functions
    initializeVars();
    lcd.splash();
    operationTicker.detach();
    resetTicker.attach(1, resetTick);
    buzzer.beep(50);
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(100);
    Serial.println(F("Initializing dishwasher program..."));
    initializeVars();
    lcd.init();
    lcd.splash();

    btn1.begin();
    btn1.onPressed(onBtn1Pressed);
    btn1.onPressedFor(1000, onBtn1LongPressed);
    btn2.begin();
    btn2.onPressed(onBtn2Pressed);

    buzzer.init(BUZZER_PIN);

    sprayMotor.init(SPRAY_MOTOR_PIN);
    drainMotor.init(DRAIN_MOTOR_PIN);
    heater.init(HEATER_PIN);

    pinMode(LOW_WATER_LEVEL_PIN, INPUT_PULLUP);

    Serial.println(F("... Setup completed"));
    buzzer.multipleBeep(100, 5);
}

void operationTick() {
    sprayMotor.invert();
    drainMotor.invert();
    heater.invert();

    if (digitalRead(LOW_WATER_LEVEL_PIN)) {
        Serial.println(F("LOW_WATER_LEVEL_PIN true"));
    } else {
        Serial.println(F("LOW_WATER_LEVEL_PIN false"));
    }

}

void resetTick() {
    sprayMotor.turnOff();
    drainMotor.turnOff();
    heater.turnOff();

    resetTicker.detach();
}

void loop() {
    btn1.read();
    btn2.read();
    buzzer.update();
}

