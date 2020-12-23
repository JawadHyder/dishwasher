//
// Created by jawad on 12/23/20.
//
enum mode {
    MODE_COLD,
    MODE_HOT
};
mode nextMode(mode m) { // to use for cycling
    return static_cast<mode>((m + 1) % 2);
};
enum duration {
    DURATION_QUICK,
    DURATION_NORMAL,
    DURATION_LONG
};
duration nextDuration(duration d) {
    return static_cast<duration>((d + 1) % 2);
}
enum state {
    STATE_WELCOME,
    STATE_MOD_SELECT,
    STATE_DURATION_SELECT,
    STATE_FUNCTIONAL
};
