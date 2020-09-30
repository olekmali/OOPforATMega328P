//* Simple Digital IO test set without further OOP *
#include "bios_keys.h"
#include "bios_leds.h"

#include <stdint.h>

# define F_CPU 16000000UL
#include <util/delay.h>


void main_toggle() {
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     disp;

    uint8_t  state = 0;

    // MAIN SUPERLOOP
    while(1)
    {
        uint8_t current = Keys.get();
        uint8_t change  = current ^ state;
        state = current;
            if ( (0 != (change & MenuKeys::Key4)) && (0 != (current & MenuKeys::Key4)) ) disp.set( disp.get() ^ LEDs::LED0 );
            if ( (0 != (change & MenuKeys::Key5)) && (0 != (current & MenuKeys::Key5)) ) disp.set( disp.get() ^ LEDs::LED1 );
            if ( (0 != (change & MenuKeys::Key6)) && (0 != (current & MenuKeys::Key6)) ) disp.set( disp.get() ^ LEDs::LED2 );
            if ( (0 != (change & MenuKeys::Key7)) && (0 != (current & MenuKeys::Key7)) ) disp.set( disp.get() ^ LEDs::LED3 );

        // let's do at least lousy debouncing
        _delay_ms(10); // from <util/delay.h>
    }
}



void main_menu() {
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     disp;

    uint8_t  state = 0;
    uint8_t  mode  = 1;
    uint8_t  prev  = 0;

    // MAIN SUPERLOOP
    while(1)
    {
        uint8_t current = Keys.get();
        uint8_t change  = current ^ state;
        state = current;
        if ( (0 != (change & MenuKeys::Key4)) && (0 != (current & MenuKeys::Key4)) ) { if (0x01<mode) mode=mode>>1; }
        if ( (0 != (change & MenuKeys::Key5)) && (0 != (current & MenuKeys::Key5)) ) {}
        if ( (0 != (change & MenuKeys::Key6)) && (0 != (current & MenuKeys::Key6)) ) {}
        if ( (0 != (change & MenuKeys::Key7)) && (0 != (current & MenuKeys::Key7)) ) { if (0x08>mode) mode=mode<<1; }

        if (mode!=prev) {
            disp.set( disp.get() ^ mode ^ prev );
            prev = mode;
        }

        // let's do at least lousy debouncing
        _delay_ms(10); // from <util/delay.h>
    }
}


int main(void)
{
    main_toggle();
    // main_menu();
    return(0);
}
