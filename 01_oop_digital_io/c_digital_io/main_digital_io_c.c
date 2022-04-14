//* Simple Digital IO test set in plain C *
#include "bios_keysc.h"
#include "bios_ledsc.h"
#include <stdint.h>

# define F_CPU 16000000UL
#include <util/delay.h>


void main_toggle() {
    uint8_t  state = 0;

    // MAIN SUPERLOOP
    while(1)
    {
        uint8_t current = keys_get();
        uint8_t change  = current ^ state;
        state = current;
            if ( (0 != (change & B_K4)) && (0 != (current & B_K4)) ) leds_set( leds_get() ^ B_L0 );
            if ( (0 != (change & B_K5)) && (0 != (current & B_K5)) ) leds_set( leds_get() ^ B_L1 );
            if ( (0 != (change & B_K6)) && (0 != (current & B_K6)) ) leds_set( leds_get() ^ B_L2 );
            if ( (0 != (change & B_K7)) && (0 != (current & B_K7)) ) leds_set( leds_get() ^ B_L3 );

        // let's do at least lousy debouncing
        _delay_ms(10); // from <util/delay.h>
    }
}



void main_menu() {
    uint8_t  state = 0;
    uint8_t  mode  = 1;
    uint8_t  prev  = 0;

    // MAIN SUPERLOOP
    while(1)
    {
        uint8_t current = keys_get();
        uint8_t change  = current ^ state;
        state = current;
        if ( (0 != (change & B_K4)) && (0 != (current & B_K4)) ) { if (0x01<mode) mode=mode>>1; }
        if ( (0 != (change & B_K5)) && (0 != (current & B_K5)) ) {}
        if ( (0 != (change & B_K6)) && (0 != (current & B_K6)) ) {}
        if ( (0 != (change & B_K7)) && (0 != (current & B_K7)) ) { if (0x08>mode) mode=mode<<1; }

        if (mode!=prev) {
            leds_set( leds_get() ^ mode ^ prev );
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
