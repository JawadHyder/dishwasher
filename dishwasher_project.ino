#include <Arduino.h>
#include <AceButton.h>
#include <Ticker.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Lcd.h"
#include "enums.h"
#include "Buzzer.h"
#include "Relay.h"

using namespace ace_button;

int ONE_SECOND_DURATION = 1000; // 1 second in ms. Reduce value to speed up program.

const int BTN_PIN_1 = 2; // Start/Fwd button.
AceButton btn1( BTN_PIN_1);

const uint8_t SPRAY_MOTOR_PIN = 0; // GPIO 0 (D3)
const uint8_t DRAIN_MOTOR_PIN = 16; // GPIO 14 (D5)
const uint8_t WATER_VALVE_PIN = 13; // GPIO 13 (D7)
const uint8_t HEATER_PIN = 15; // GPIO 15 
Relay_Controller sprayMotor;
Relay_Controller drainMotor;
Relay_Controller waterValve;
Relay_Controller heater;


const uint8_t TEMPERATURE_SENSOR_PIN = 14; // GPIO 16 (D0)
const uint8_t LOW_WATER_LEVEL_PIN = 3; // low water level
const uint8_t HIGH_WATER_LEVEL_PIN = 12; // GPIO 12 (D6)

OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature sensors(&oneWire);


dw_mode CURRENT_MODE;
dw_duration CURRENT_DURATION;
dw_state CURRENT_STATE;
dw_cycle CURRENT_CYCLE;
long cycleStartTime;
bool drining;

LCD_Controller lcd; // Initialize LCD

// Ticks every second when program is in functional state
Ticker operationTicker;
Ticker resetTicker;

void initializeVars() {
    CURRENT_MODE = MODE_HOT;
    CURRENT_DURATION = DURATION_QUICK;
    CURRENT_STATE = STATE_WELCOME;
    CURRENT_CYCLE = CYCLE_1;
    cycleStartTime = 0;
    drining = true;
}

void onBtn1Clicked() {
    Serial.println("Button1 has been clicked!");
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
    // buzzer.beep(50);
}

void onBtn1DoubleClicked() {
    Serial.println("Button1 has been double clicked!");
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
            operationTicker.attach_ms(ONE_SECOND_DURATION, operationTick);
            break;
    }
    // buzzer.beep(200);
}

void onBtn1LongPressed() {
    Serial.println("Button1 has been long pressed!");
    // Stop all functions
    initializeVars();
    lcd.splash();
    operationTicker.detach();
    resetTicker.attach(1, resetTick);
    // buzzer.beep(50);
}

// The event handler for the button.
void handleButtonEvent(AceButton* /* button */, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
  Serial.print(F("handleButtonEvent(): eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  switch (eventType) {
    case AceButton::kEventClicked:
        onBtn1Clicked();
        break;
    case AceButton::kEventDoubleClicked:
        onBtn1DoubleClicked();
        break;
    case AceButton::kEventLongPressed:
        onBtn1LongPressed();
        break;
  }
}

void setup() {
    Serial.begin(9600);
    while (! Serial) delay(100);
    Serial.println(F("Initializing dishwasher program..."));
    initializeVars();
    lcd.init();
    lcd.splash();

    pinMode(BTN_PIN_1, INPUT_PULLUP);
    ButtonConfig* buttonConfig = btn1.getButtonConfig();
    buttonConfig->setEventHandler(handleButtonEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);

    // buzzer.init(BUZZER_PIN);

    sprayMotor.init(SPRAY_MOTOR_PIN);
    drainMotor.init(DRAIN_MOTOR_PIN);
    heater.init(HEATER_PIN);
    waterValve.init(WATER_VALVE_PIN);

    pinMode(LOW_WATER_LEVEL_PIN, INPUT_PULLUP);
    // pinMode(HIGH_WATER_LEVEL_PIN, INPUT_PULLUP);

    sensors.setWaitForConversion(false);
    sensors.begin();

    Serial.println(F("... Setup completed"));
    // buzzer.multipleBeep(100, 5);
}

bool nextCycle() {
    cycleStartTime = 0;
    switch (CURRENT_CYCLE) {
        case CYCLE_1: 
            Serial.println(F("Changing cycle to 2..."));
            CURRENT_CYCLE = CYCLE_2;
            return true;
        case CYCLE_2: 
            Serial.println(F("Changing cycle to 3..."));
            CURRENT_CYCLE = CYCLE_3;
            return true;
        case CYCLE_3: 
            Serial.println(F("Cycle 3 completed. Resetting..."));
            initializeVars();
            operationTicker.detach();
            resetTicker.attach(1, resetTick);
            lcd.splash();
            return false;
    }
}

void operationTickTest() {
    static int count_relay = 0;
    count_relay++;

    sensors.requestTemperatures();
    while(!sensors.isConversionComplete()) {
        // Just waiting for sensor to complete conversion
    }
    float temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");

    if (count_relay == 1)
    {
        sprayMotor.turnOn();
        drainMotor.turnOff();
        waterValve.turnOff();
        heater.turnOff();
    } else if (count_relay == 2)
    {
        sprayMotor.turnOff();
        drainMotor.turnOn();
        waterValve.turnOff();
        heater.turnOff();
    }else if (count_relay == 3)
    {
        sprayMotor.turnOff();
        drainMotor.turnOff();
        waterValve.turnOn();
        heater.turnOff();
    }
    else if (count_relay == 4)
    {
        sprayMotor.turnOff();
        drainMotor.turnOff();
        waterValve.turnOff();
        heater.turnOn();
    }
    else {
        sprayMotor.turnOff();
        drainMotor.turnOff();
        waterValve.turnOff();
        heater.turnOff();
        count_relay = 0;
    }
    Serial.println(F("Test tick..."));
}

void operationTick() {

    bool waterLow = digitalRead(LOW_WATER_LEVEL_PIN);
    bool waterHigh = false;//digitalRead(HIGH_WATER_LEVEL_PIN);

    if (drining) {
        Serial.println(F("Draining..."));
        drainMotor.turnOn();
        if (!waterLow) {
            Serial.println(F("Draining completed..."));
            drining = false;
            drainMotor.turnOff();
        }
        return;
    }
    
    if (!waterHigh) {
        Serial.println(F("Water not full..."));
        waterValve.turnOn();
        sprayMotor.turnOff();
        return;
    }
    waterValve.turnOff();

    cycleStartTime++; // add one second to cycleStartTime counter
    Serial.print(cycleStartTime);
    Serial.print(F("seconds lapsed in "));
    Serial.print(CURRENT_CYCLE);
    Serial.println(F(" cycle..."));
    
    sprayMotor.turnOn();
    
    if ((CURRENT_DURATION == DURATION_QUICK && cycleStartTime >= 390 /* 6.5 mins for each cycle (3 cycles = 20 mins)*/)
        || (CURRENT_DURATION == DURATION_QUICK && cycleStartTime >= 810 /* 13.5 mins for each cycle (3 cycles = 40 mins) */)
        || (CURRENT_DURATION == DURATION_QUICK && cycleStartTime >= 1200 /* 20 mins for each cycle (3 cycles = 60 mins) */)) {
        
        Serial.println(F("Turning off spray..."));
        sprayMotor.turnOff();
        drining = true;
        nextCycle();
    }

}

void resetTick() {
    sprayMotor.turnOff();
    drainMotor.turnOff();
    waterValve.turnOff();
    heater.turnOff();

    resetTicker.detach();
}

void loop() {
    btn1.check();

    // sensors.requestTemperatures();
    // float temperatureC = sensors.getTempCByIndex(0);
    // Serial.print(temperatureC);
    // Serial.println("ºC");

    // delay(1000);
    // btn2.read();
    // buzzer.update();
}

