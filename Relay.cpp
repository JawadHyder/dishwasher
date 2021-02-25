//
// Created by jawad on Jan/02/21.
//
#include <Arduino.h>
#include "Relay.h"

Relay_Controller::Relay_Controller() {}

void Relay_Controller::init(uint8_t pinNum) {
    Relay_Controller_PIN_NUM = pinNum;
    pinMode(Relay_Controller_PIN_NUM, OUTPUT);
    digitalWrite(Relay_Controller_PIN_NUM, HIGH);
}
void Relay_Controller::turnOn(){
    // Serial.println(F("Turning on relay..."));
    digitalWrite(Relay_Controller_PIN_NUM, LOW);
}
void Relay_Controller::turnOff(){
    // Serial.println(F("Turning off relay..."));
    digitalWrite(Relay_Controller_PIN_NUM, HIGH);
}
void Relay_Controller::invert(){
    // Serial.println(F("Inverting relay..."));
    digitalWrite(Relay_Controller_PIN_NUM, !digitalRead(Relay_Controller_PIN_NUM));
}
