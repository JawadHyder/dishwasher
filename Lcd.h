#include <Arduino.h>
#include "enums.h"
//
// Created by jawad on 12/22/20.
//

class LCD_Controller {
public:
    LCD_Controller();
    void init();
    void splash();
    void showMode(dw_mode m);
    void showDuration(dw_duration d);
    void showConfirmation(dw_mode m, dw_duration d);
    void showFunctionalDetails();
private:
    void clearWhite();
    void clearBlack();
};