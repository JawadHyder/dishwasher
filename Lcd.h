#include <Arduino.h>
//
// Created by jawad on 12/22/20.
//

class LCD_Controller {
public:
    LCD_Controller();
    void init();
    void splash();
    void showMode(uint8_t m);
    void showDuration(uint8_t d);

private:
    void clearWhite();
    void clearBlack();
};