//* lib_buttons.cpp - sample state machine library *
#include "lib_buttons.h"


void DebounceFilter::update(bool pressed) {
    if (pressed) {
        count_on++;
        if (count_on>threshold) {
            count_off=0;
            state=true;
        }
    }
    else {
        count_off++;
        if (count_off>threshold) {
            count_on=0;
            state=false;
        }
    }
}



void ToggleButton::update(bool pressed) {
    if (pressed!=recentinput && pressed==true) {
        state = !state; // negate (flip) the state
    }
    recentinput=pressed;
}



void EdgeDetectorFilter::update(bool pressed) {
    if (pressed!=recentinput && recentinput==onfallingedge) {
        raised = true;
    }
    recentinput=pressed;
}

bool EdgeDetectorFilter::getState() {
    bool status=raised;
    raised=false;
    return(status);
}
