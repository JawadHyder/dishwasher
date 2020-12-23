//
// Created by jawad on 12/23/20.
//
#ifndef ARDUINO_ENUMS_H
#define ARDUINO_ENUMS_H

enum dw_mode {
    MODE_COLD,
    MODE_HOT
};
static dw_mode nextDwMode(dw_mode m) { // to use for cycling
    return static_cast<dw_mode>((m + 1) % 2);
};
enum dw_duration {
    DURATION_QUICK,
    DURATION_NORMAL,
    DURATION_LONG
};
static dw_duration nextDwDuration(dw_duration d) {
    return static_cast<dw_duration>((d + 1) % 3);
}
enum state {
    STATE_WELCOME,
    STATE_MOD_SELECT,
    STATE_DURATION_SELECT,
    STATE_CONFIRMATION,
    STATE_FUNCTIONAL
};

#endif