//* Simple Digital IO test set in plain C *
#include "bios_keysc.h"
#include "bios_ledsc.h"

#include <stdint.h>

# define F_CPU 16000000UL
#include <util/delay.h>


uint8_t choose_on_power_on(uint8_t wait) {
    uint8_t     result=0;
    do {
        result = keys_get();
    } while ( wait && (0==result) );

    if (0!=result) {
        uint8_t waitst=0;
        while ( waitst<64 ) {
            if ( 0 != keys_get() ) waitst=0;
            else waitst++;
        }
    }
    return(result);
}



void main_toggle1() {
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



void main_menu1() {
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


void main_blank1() {
    while (1) ;
}


void main_blank2() {
    while (1) ;
}


int main(void)
{
    // INITIALIZATION
    leds_init();
    keys_init();

    switch ( choose_on_power_on(1) ) {
    case B_K4 :
//      leds_set(B_L5);
        main_toggle1();
        break;
    case B_K5 :
//      leds_set(B_L5|B_L6);
        main_menu1();
        break;
    case B_K6 :
//      leds_set(B_L5|B_L7);
        main_blank1();
        break;
    case B_K7 : 
//      leds_set(B_L5|B_L6|B_L7);
        main_blank2();
        break;
    }

    return(0);
}
