//* lib_buttons.h - sample state machine library *
#ifndef LIB_BUTTONS_H_
#define LIB_BUTTONS_H_
#include <stdint.h>



class DebounceFilter {
public:
    DebounceFilter(bool inistate=false) : state(inistate), count_off(0), count_on(0) {}
    void update(bool pressed);
    bool getState() const   { return(state); }
private:
    bool    state;
    uint8_t count_off;
    uint8_t count_on;
    const static uint8_t threshold = 64; // warning: hardware specific
};



class ToggleButton {
public:
    ToggleButton(bool inistate=false) : state(inistate), recentinput(false) {}
    void update(bool pressed);
    bool getState() const   { return(state); }
private:
    bool state;
    bool recentinput;
};



class EdgeDetectorFilter {
public:
    EdgeDetectorFilter(bool detectFalling = true)
        : raised(false), recentinput(false), onfallingedge(detectFalling) {}
    void update(bool pressed);
    bool getState();
private:
    bool raised;
    bool recentinput;
    bool onfallingedge;
};



#endif /* LIB_BUTTONS_H_ */
