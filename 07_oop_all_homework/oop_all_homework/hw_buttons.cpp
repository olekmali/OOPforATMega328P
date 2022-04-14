//* This is a "stub" file that can be replaced by the implementation file for a fully functional library for push buttons *
#include "hw_buttons.h"


void DebounceFilter::update(bool pressed) {
	if (pressed) {
		count_on++;
		if (count_on>threshold) { count_off = 0; state = true; }
	}
	else {
		count_off++;
		if (count_off>threshold) { count_on = 0; state = false; }
	}
}


void AutorepeaterFilter::update(bool pressed) {
	if (pressed) {
		state = true;
		count_on++;
		if (count_on>threshold) { count_on = 0; state = false; }
	}
	else {
		count_on = 0;
		state = false;
	}
}

PushButton::PushButton(bool inistate)
: state(inistate) {
}

TogglePushButton::TogglePushButton(bool inistate)
: PushButton(inistate), recentinput(inistate) {
}

void TogglePushButton::update(bool pressed) {
	if (pressed != recentinput && pressed == true) {
		state = !state; // negate (flip) the state
	}
	recentinput = pressed;
}

DebouncedTogglePushButton::DebouncedTogglePushButton(bool inistate)
: TogglePushButton(inistate), debouncer(inistate) {
}

void DebouncedTogglePushButton::update(bool pressed) {
	debouncer.update(pressed);
	TogglePushButton::update(debouncer.getState());
}

// TODO
// .. AutorepeatDebouncedTogglePushButton from the recent homework assignment - implementation file portion
