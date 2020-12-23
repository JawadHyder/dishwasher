//
// Created by jawad on 12/22/20.
//
#include <Arduino.h>
#include "Lcd.h"
#include "enums.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>

// Used for I2C or SPI
#define OLED_RESET LED_BUILTIN
// I2C
Adafruit_SSD1305 display(130, 64, &Wire, OLED_RESET);

LCD_Controller::LCD_Controller() {}

void LCD_Controller::init() {
    if ( ! display.begin(0x3C) ) {
        Serial.println("Unable to initialize OLED");
        while (1) yield();
    }
    clearWhite();
    delay(100);
    clearBlack();
    display.display();
    Serial.println(F("... Initialized LCD"));
    delay(100);
}
void LCD_Controller::splash() {
    Serial.println(F("Showing splash screen..."));
    clearBlack();
    display.clearDisplay();
    display.setTextSize(1.9);
    display.setTextColor(WHITE);
    display.setCursor(10,20);
    display.println("Dishwasher");
    display.setCursor(10,30);
    display.println("Ready...");
    display.display();
    Serial.println(F("... Drew splash"));
}
void LCD_Controller::showMode(dw_mode m) {
    clearBlack();
    display.setTextSize(1.9);
    display.setTextColor(WHITE);
    display.setCursor(5,10);
    display.println("Select Mode:");
    display.setTextSize(2);
    display.setCursor(5,20);
    switch (m) {
        case MODE_COLD:
            display.println("COLD");
            break;
        case MODE_HOT:
            display.println("HOT");
            break;
    }
    display.display();
}
void LCD_Controller::showDuration(dw_duration d) {
    clearBlack();
    display.setTextSize(1.9);
    display.setTextColor(WHITE);
    display.setCursor(5,10);
    display.println("Select Duration:");
    display.setTextSize(2);
    display.setCursor(5,20);
    switch (d) {
        case DURATION_QUICK:
            display.println("QUICK");
            break;
        case DURATION_NORMAL:
            display.println("NORMAL");
            break;
        case DURATION_LONG:
            display.println("LONG");
            break;
    }
    display.display();
}
void LCD_Controller::showConfirmation(dw_mode m, dw_duration d) {
    clearBlack();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5,5);
    display.print("M:");
    switch (m) {
        case MODE_HOT:
            display.println("HOT");
            break;
        case MODE_COLD:
            display.println("COLD");
            break;
    }
    display.setCursor(5,25);
    display.print("D:");
    switch (d) {
        case DURATION_QUICK:
            display.println("QUICK");
            break;
        case DURATION_NORMAL:
            display.println("NORMAL");
            break;
        case DURATION_LONG:
            display.println("LONG");
            break;
    }
    display.setCursor(5,50);
    display.setTextSize(1);
    display.println("Long press...");
    display.display();
}
void LCD_Controller::showFunctionalDetails() {
    clearBlack();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5,10);
    display.println("Functional...");
    display.display();
}
void LCD_Controller::clearWhite() {
    display.clearDisplay();
    display.fillRect(0, 0, display.width() -1, display.height() -1, WHITE);
    display.display();
}
void LCD_Controller::clearBlack() {
    display.clearDisplay();
    display.fillRect(0, 0, display.width() -1, display.height() -1, BLACK);
}
