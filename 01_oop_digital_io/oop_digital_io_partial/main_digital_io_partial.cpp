//* Simple Digital IO test set without further OOP *
#include "bios_keys.h"
#include "bios_leds.h"

#include <stdint.h>

# define F_CPU 16000000UL
#include <util/delay.h>


uint8_t choose_on_power_on(bool wait=true) {
    MenuKeys    Keys;
    uint8_t     result=0;
    do {
        result = Keys.get();
    } while ( wait && (0==result) );

    if (0!=result) {
        uint8_t waitst=0;
        while ( waitst<64 ) {
            if ( 0 != Keys.get() ) waitst=0;
            else waitst++;
        }
    }
    return(result);
}



void main_toggle1() {
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



void main_menu1() {
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


void main_blank1() {
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     disp;
    while (1) ;
}


void main_blank2() {
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     disp;
    while (1) ;
}


int main(void)
{
    LEDs diag;

    switch ( choose_on_power_on() ) {
    case MenuKeys::Key4 :
//      diag.set(LEDs::LED5);
        main_toggle1();
        break;
    case MenuKeys::Key5 :
//      diag.set(LEDs::LED5|LEDs::LED6);
        main_menu1();
        break;
    case MenuKeys::Key6 :
//      diag.set(LEDs::LED5|LEDs::LED7);
        main_blank1();
        break;
    case MenuKeys::Key7 : 
//      diag.set(LEDs::LED5|LEDs::LED6|LEDs::LED7);
        main_blank2();
        break;
    }

    return(0);
}
