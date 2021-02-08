#include <Arduino.h>
//
// Created by jawad on Jan/02/21.
//

class Relay_Controller {
public:
    Relay_Controller();
    void init(uint8_t pinNum);
    void turnOn();
    void turnOff();
    void invert();
private:
	uint8_t Relay_Controller_PIN_NUM;
};