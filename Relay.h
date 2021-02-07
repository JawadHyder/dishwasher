#include <Arduino.h>
//
// Created by jawad on Jan/02/21.
//

class Relay_Controller {
public:
    Relay_Controller();
    void init(int pinNum);
    void turnOn();
    void turnOff();
    void invert();
private:

};