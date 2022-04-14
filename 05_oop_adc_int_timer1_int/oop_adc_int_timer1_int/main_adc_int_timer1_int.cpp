/*
 * OOP Case study: oop_adc_int_timer1_int
 *
 * Created: 9/9/2020 1:27:00 AM
 * Updated: 11/29/2021 11:13:00 PM
 *  Author: Aleksander Malinowski
 */

#include "lib_atomic.h"
#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_timer1.h"
#include "bios_adc.h"

#include <stdlib.h>
#include <avr/interrupt.h>

class myInterrupt : public int_abase {
public:
    virtual void    handle();
    myInterrupt() : counter(0) {}
private:
    uint8_t counter;
};


void myInterrupt::handle() {
    // You are inside interrupt
    // Avoid:
    //  loops with undetermined delay
    //  or anything computationally intense
    //      including use of integer * and /
    //  not to mention anything floating point
    if (counter==0)
    {
        counter = 10;
        // HERE do things every so many interrupts
        ADConverter::getInstance().startRound();
    } else {
        counter--;
    }
}


int main(void)
{
    // INITIALIZATION
    MenuKeys Keys;
    LEDs     LEDs;

    myInterrupt myInt;

    //Timer1   &T1 = Timer1::getInstance();
    //T1.initialize(1000, &myInt, Timer1::timer_prescale_1);
    Timer1::getInstance().initialize(1000, &myInt, Timer1::timer_prescale_1);

    ADConverter &V1 = ADConverter::getInstance();
    V1.initialize();

    sei();
    // MAIN SUPERLOOP
    while(1)
    {
        while ( !V1.isReady() ) ;
        V1.resetReady();

        if (Keys.get() & MenuKeys::Key4)
            LEDs.set(V1.getRecent(ADConverter::CH0)>>2);
        else if (Keys.get() & MenuKeys::Key5)
            LEDs.set(V1.getRecent(ADConverter::CH0));
        else if (Keys.get() & MenuKeys::Key6)
            LEDs.set(V1.getRecent(ADConverter::CH1)>>2);
        else if (Keys.get() & MenuKeys::Key7)
            LEDs.set(V1.getRecent(ADConverter::CH1));
        else ; // otherwise keep the most recent value

    }

    return(0);
}
