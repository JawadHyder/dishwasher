#include <Arduino.h>
#include "enums.h"
//
// Created by jawad on Jan/02/21.
//

class Buzzer_Controller {
public:
    Buzzer_Controller();
    void init(int pinNum);
    void beep(unsigned long ms);
    void multipleBeep(unsigned long ms, int times);
    void update();
private:
    unsigned long timeSpent();
};