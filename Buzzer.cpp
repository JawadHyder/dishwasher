//
// Created by jawad on Jan/02/21.
//
#include <Arduino.h>
#include "Buzzer.h"

unsigned long Buzzer_Controller_PIN_NUM = 0;
bool Buzzer_Controller_beep_status = false;
unsigned long Buzzer_Controller_timerStartMs = 0;
int Buzzer_Controller_beepCount = 0;
unsigned long Buzzer_Controller_beepDurationMs = 0;

Buzzer_Controller::Buzzer_Controller() {}

void Buzzer_Controller::init(int pinNum) {
    Buzzer_Controller_PIN_NUM = pinNum;
    // pinMode(Buzzer_Controller_PIN_NUM, OUTPUT);
    digitalWrite(Buzzer_Controller_PIN_NUM, LOW);
}
void Buzzer_Controller::beep(unsigned long ms) {
    Buzzer_Controller_timerStartMs = millis();
    Buzzer_Controller_beepDurationMs = ms;
    Buzzer_Controller_beep_status = true;
    pinMode(Buzzer_Controller_PIN_NUM, FUNCTION_3); // Hack to use TX as output. FUNCTION_3 will convert tx to GPIO pin
    pinMode(Buzzer_Controller_PIN_NUM, OUTPUT);
    digitalWrite(Buzzer_Controller_PIN_NUM, HIGH);
    Buzzer_Controller_beepCount = max(Buzzer_Controller_beepCount - 1, 0);
}
void Buzzer_Controller::multipleBeep(unsigned long ms, int times) {
//    if (Buzzer_Controller_beeping)
//        return;
//    Serial.println(F("Starting multiple beep"));
//    Buzzer_Controller_beepCount = times;
//    singleBeep(ms);
}
void Buzzer_Controller::update() {
//    Serial.println(F("-------------------------------------"));
//    Serial.print(F("update: Buzzer_Controller_beeping: "));
//    Serial.println(Buzzer_Controller_beeping);
//    Serial.print(F("update: Buzzer_Controller_beepStartAt: "));
//    Serial.println(Buzzer_Controller_beepStartAt);
//    Serial.print(F("update: Buzzer_Controller_beepCount: "));
//    Serial.println(Buzzer_Controller_beepCount);
//    Serial.print(F("update: Buzzer_Controller_beepWaiting: "));
//    Serial.println(Buzzer_Controller_beepWaiting);
//    Serial.print(F("update: Buzzer_Controller_beepGapTimer: "));
//    Serial.println(Buzzer_Controller_beepGapTimer);
//    Serial.print(F("update: millis(): "));
//    Serial.println(millis());
//    Serial.println(F("-------------------------------------"));

    if (Buzzer_Controller_beep_status && (timeSpent() > Buzzer_Controller_beepDurationMs)) {
        Buzzer_Controller_timerStartMs = 0;
        Buzzer_Controller_beepDurationMs = 0;
        Buzzer_Controller_beep_status = false;
        digitalWrite(Buzzer_Controller_PIN_NUM, LOW);
        pinMode(Buzzer_Controller_PIN_NUM, FUNCTION_0); // Hack to use TX as output. FUNCTION_0 will convert tx from GPIO back to TX pin
    }
//    else if (Buzzer_Controller_beepCount > 0) {
//        Serial.println(F("update: Buzzer_Controller_beepCount is not 0"));
//        if (Buzzer_Controller_beepWaiting && (millis() - Buzzer_Controller_beepGapTimer > Buzzer_Controller_beepGap)) {
//            Serial.println(F("update: Buzzer_Controller_beepWaiting completed starting another beep"));
//            Buzzer_Controller_beepWaiting = false;
//            singleBeep(Buzzer_Controller_beepFor);
//        } else {
//            Serial.println(F("update: setting Buzzer_Controller_beepWaiting"));
//            Buzzer_Controller_beepWaiting = true;
//            Buzzer_Controller_beepGapTimer = millis();
//        }
//    }
}

unsigned long Buzzer_Controller::timeSpent() {
    return (millis() - Buzzer_Controller_timerStartMs);
}