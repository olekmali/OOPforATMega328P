//* This is a "stub" file that can be replaced by the header file for a fully functional library for push buttons *
#ifndef HW_BUTTONS_H_
#define HW_BUTTONS_H_
#include <stdint.h>


class DebounceFilter {
public:
    DebounceFilter(bool inistate = false) : state(inistate), count_off(0), count_on(0) {}
    void    update(bool pressed);
    bool    getState() const { return(state); }
private:
    bool    state;
    uint8_t count_off;
    uint8_t count_on;
    const static uint8_t threshold = 64; // warning: hardware specific
};

class AutorepeaterFilter {
public:
    AutorepeaterFilter(bool inistate = false) : state(inistate), count_on(0) {}
    void    update(bool pressed);
    bool    getState() const { return(state); }
private:
    bool    state;
    uint16_t count_on;
    const static uint16_t threshold = 16384; // warning: sampling rate specific
};

class PushButton {
public:
    PushButton(bool inistate = false);
    virtual void update(bool pressed) = 0;
public:
    bool    getState() const { return(state); }
protected:
    bool    state;
};


class TogglePushButton : public PushButton {
public:
    TogglePushButton(bool inistate = false);
    virtual void update(bool pressed);
protected:
    bool recentinput;
};

class DebouncedTogglePushButton : public TogglePushButton {
public:
    DebouncedTogglePushButton(bool inistate = false);
    virtual void update(bool pressed);
protected:
    DebounceFilter debouncer;
};

// TODO
// .. AutorepeatDebouncedTogglePushButton from the recent homework assignment - header file portion

#endif /* LIB_BUTTONS_H_ */
