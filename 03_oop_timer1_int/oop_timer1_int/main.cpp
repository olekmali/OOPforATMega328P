/*
 * OOP Case study: oop_timer1_int
 *
 * Created: 9/9/2020 1:27:00 AM
 *  Author: Aleksander Malinowski
 */ 

#include "lib_atomic.h"
#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_timer1.h"

#include <stdlib.h>
#include <avr/interrupt.h>


Atomic semaphore;

class myInterrupt : public int_abase {
public:
    virtual void    handle();

    myInterrupt(uint16_t param);
    void            set(uint16_t param);
    uint16_t        get();
    
private:
    uint16_t        counter;
    uint16_t        frequency;
    // handling changes at roll over
    uint8_t         changed; // 1 byte Boolean (atomic set)
    uint16_t        newfreq;
};


void myInterrupt::handle() {
    // You are inside interrupt
    // Avoid:
    //  loops with undetermined delay 
    //  or anything computationally intense
    //      including use of integer * and /
    //  not to mention anything floating point
    if (counter>0) {
        --counter;
    } else {
        if (changed) {
            frequency = newfreq;
            changed = false;
        }
        counter = frequency;
        semaphore.set(); 
    }
}

myInterrupt::myInterrupt(uint16_t param) 
    : counter(0), frequency(param), changed(false) {}

void myInterrupt::set(uint16_t param) {
    cli();
    // disable interrupts if more data than one uint8_t variable is changed at the same time
    newfreq = param;
    changed = true;
    sei();
}

uint16_t myInterrupt::get() {
    uint16_t result;
    cli();
    // disable interrupts if more data than one uint8_t variable is returned
    result = counter;
    sei();
    return(result);
}



int main(void)
{
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     LEDs;
    
    myInterrupt I(5);
    
    Timer1   &T1 = Timer1::getInstance();
    T1.initialize(10, &I, Timer1::timer_prescale_1024);

    sei();
    // MAIN SUPERLOOP
    while(1)
    {
        while ( 0==semaphore.check() )  ;
        // Note: we are relying on the fact that the semaphore will not be immediately set
        //      Otherwise, a function try_to_set() would be necessary instead of the wait-n-clear 
        semaphore.clear();
        
        LEDs.set( LEDs.get() ^ LEDs.LED0 );
        if ( (Keys.get() & MenuKeys::Key4)!=0 ) I.set(5);
        if ( (Keys.get() & MenuKeys::Key5)!=0 ) I.set(1);
    }
    
    return(0);
}
