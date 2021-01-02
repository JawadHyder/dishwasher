//
// Created by jawad on Jan/02/21.
//
#include <Arduino.h>
#include "Buzzer.h"

bool Buzzer_Controller_beeping = false;
unsigned long Buzzer_Controller_beepStartAt = 0;
unsigned long Buzzer_Controller_beepFor = 0;
int Buzzer_Controller_beepCount = 0;
unsigned long Buzzer_Controller_PIN_NUM = 0;
unsigned long Buzzer_Controller_beepGap = 100;
unsigned long Buzzer_Controller_beepGapTimer = 0;
bool Buzzer_Controller_beepWaiting = false;

Buzzer_Controller::Buzzer_Controller() {}

void Buzzer_Controller::init(int pinNum) {
    Buzzer_Controller_PIN_NUM = pinNum;
    pinMode(Buzzer_Controller_PIN_NUM, OUTPUT);
    digitalWrite(Buzzer_Controller_PIN_NUM, LOW);
}
void Buzzer_Controller::singleBeep(unsigned long ms) {
    Serial.println(F("in single beep"));
    if (Buzzer_Controller_beeping)
        return;
    Buzzer_Controller_beeping = true;
    Buzzer_Controller_beepStartAt = millis();
    Buzzer_Controller_beepFor = ms;
    digitalWrite(Buzzer_Controller_PIN_NUM, HIGH);
    Buzzer_Controller_beepCount = (Buzzer_Controller_beepCount > 0) ?
            Buzzer_Controller_beepCount - 1 : 0;
    Serial.print(F("Buzzer_Controller_beepCount set to: "));
    Serial.println(Buzzer_Controller_beepCount);
}
void Buzzer_Controller::multipleBeep(unsigned long ms, int times) {
    if (Buzzer_Controller_beeping)
        return;
    Serial.print(F("Starting multiple beep"));
    Buzzer_Controller_beepCount = times;
    singleBeep(ms);
}
void Buzzer_Controller::update() {
    if (Buzzer_Controller_beeping && ((millis() - Buzzer_Controller_beepStartAt) > Buzzer_Controller_beepFor)) {
        Serial.print(F("update: Buzzer_Controller_beeping is true"));
        Buzzer_Controller_beeping = false;
        Buzzer_Controller_beepStartAt = 0;
        Buzzer_Controller_beepFor = 0;
        digitalWrite(Buzzer_Controller_PIN_NUM, LOW);
    } else if (Buzzer_Controller_beepCount > 0) {
        Serial.print(F("update: Buzzer_Controller_beepCount is not 0"));
        if (Buzzer_Controller_beepWaiting && (millis() - Buzzer_Controller_beepGapTimer > Buzzer_Controller_beepGap)) {
            Serial.print(F("update: Buzzer_Controller_beepWaiting completed starting another beep"));
            Buzzer_Controller_beepWaiting = false;
            singleBeep(Buzzer_Controller_beepFor);
        } else {
            Serial.print(F("update: setting Buzzer_Controller_beepWaiting"));
            Buzzer_Controller_beepWaiting = true;
            Buzzer_Controller_beepGapTimer = millis();
        }
    }
}
