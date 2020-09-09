//*  Digital IO test set with OOP button no for loop examples *
#include "bios_keys.h"
#include "bios_leds.h"
#include "lib_buttons.h"

#include <stdint.h>



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

    DebounceFilter  BF0, BF1, BF2, BF3;
    ToggleButton    B0, B1, B2, B3;

    // MAIN SUPERLOOP
    while(1)
    {
        BF0.update( 0!= (Keys.get() & MenuKeys::Key4) );
        B0.update( BF0.getState() );
        if ( B0.getState() )    disp.set( disp.get() | LEDs::LED0 );
        else                    disp.set( disp.get() & ~LEDs::LED0 );

        BF1.update( 0!= (Keys.get() & MenuKeys::Key5) );
        B1.update( BF1.getState() );
        if ( B1.getState() )    disp.set( disp.get() | LEDs::LED1 );
        else                    disp.set( disp.get() & ~LEDs::LED1 );

        BF2.update( 0!= (Keys.get() & MenuKeys::Key6) );
        B2.update( BF2.getState() );
        if ( B2.getState() )    disp.set( disp.get() | LEDs::LED2 );
        else                    disp.set( disp.get() & ~LEDs::LED2 );

        BF3.update( 0!= (Keys.get() & MenuKeys::Key7) );
        B3.update( BF3.getState() );
        if ( B3.getState() )    disp.set( disp.get() | LEDs::LED3 );
        else                    disp.set( disp.get() & ~LEDs::LED3 );

    }
}



void main_menu1() {
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     disp;

    DebounceFilter     leftButton;
    DebounceFilter     rightButton(false);
    EdgeDetectorFilter left;
    EdgeDetectorFilter right;

    // MAIN SUPERLOOP
    uint8_t mode = 1;
    uint8_t prev = 0;
    while(1)
    {
        leftButton.update(    0!= (Keys.get() & MenuKeys::Key4) );
        rightButton.update(   0!= (Keys.get() & MenuKeys::Key7) );
        left.update(  leftButton.getState() );
        right.update( rightButton.getState() );

        if ( left.getState() )  { if (0x01<mode) mode=mode>>1; }
        if ( right.getState() ) { if (0x08>mode) mode=mode<<1; }

        if (mode!=prev) {
            disp.set( disp.get() ^ mode ^ prev );
            prev = mode;
        }
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
