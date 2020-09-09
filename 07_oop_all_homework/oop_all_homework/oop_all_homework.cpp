/*
 * OOP Case study: all features test
 *
 * Created: 9/9/2020 1:27:00 AM
 *  Author: Aleksander Malinowski
 */

#include "lib_atomic.h"
#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_timer1.h"
#include "bios_adc.h"
#include "lib_pwm.h"
#include "lib_buttons.h"

#include "hw_buffer.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


class myInterrupt : public int_abase {
public:
    virtual void    handle();
    myInterrupt(uint16_t ADC_interval, PWM_Generator<4, uint8_t>* GEN, LEDs* PWM_Pins)
        : interval(ADC_interval), counter(0), PWM_GEN(GEN), PWM_PIN(PWM_Pins) {}
private:
    uint16_t                    interval;
    uint16_t                    counter;
    PWM_Generator<4, uint8_t>*  PWM_GEN;
    LEDs*                       PWM_PIN;
};

void myInterrupt::handle() {
    // You are inside interrupt
    // Avoid:
    //  loops with undetermined delay
    //  or anything computationally intense
    //      including use of integer * and /
    //  not to mention anything floating point


    PWM_PIN->set( PWM_GEN->NextStepUpdate( PWM_PIN->get() ) );

    if (counter==0)
    {
        counter = interval;
        // HERE do things every so many interrupts
        ADConverter::getInstance().startRound();
    } else {
        counter--;
    }
}

int main(void)
{
    wdt_enable(3);
    // now you must reset watchdog more frequently than:
    // 0 - 14.0ms
    // 1 - 28.1ms
    // 2 - 56.2ms
    // 3 - 0.11s
    // 4 - 0.22s
    // 5 - 0.45s
    // 6 - 0.9s
    // 7 - 1.8s

    LEDs LED;
    LED.set(0);

    MenuKeys KEY;
    ToggleButton B0;
    // ^^^^^^^^^ Convert to autorepeat button per request in the code below
    uint8_t alive = 0;

    PWM_Generator<4, uint8_t> PWM(100, 100);

    myInterrupt myInt(10000/50, &PWM, &LED);

    Timer1::getInstance().initialize(10000, &myInt, Timer1::timer_prescale_1);

    ADConverter &V1 = ADConverter::getInstance();
    V1.initialize();

    CircularBuffer<uint8_t, uint8_t, uint8_t, 1, 1> Channel0, Channel1;
    //             ^^ replace with the correct data type for data storage
    //                      ^^ replace with the correct data type for the sum of data to be averaged
    //                               ^^ replace with the correct data type for the size used with this buffer
    //                                        ^^     compute 50Hz x 20 seconds and set the buffer size/long average capacity
    //                                           ^^  compute 50Hz x 5 seconds and set the short average capacity

    sei();
    // MAIN SUPERLOOP
    while(1)
    {
        while ( !V1.isReady() ) ;
        V1.resetReady();

        Channel0.store( V1.getRecent(ADConverter::CH4) );
        Channel1.store( V1.getRecent(ADConverter::CH5) );

        // if not buffer ready keep averages at 0
        // otherwise compute the averages using two
        // custom circular buffer templates of <uint8_t>

        if ( Channel0.ready() )
        {
            PWM.setPWM(0, static_cast<uint16_t>(100)*Channel0.ShortAverage()/255);
            PWM.setPWM(1, static_cast<uint16_t>(100)*Channel0.LongAverage()/255);
        } else {
            PWM.setPWM(0, 0);
            PWM.setPWM(1, 0);
        }
        if ( Channel1.ready() )
        {
            PWM.setPWM(2, static_cast<uint16_t>(100)*Channel1.ShortAverage()/255);
            PWM.setPWM(3, static_cast<uint16_t>(100)*Channel1.LongAverage()/255);
        } else {
            PWM.setPWM(2, 0);
            PWM.setPWM(3, 0);
        }

        if ( 0 != (KEY.get() & MenuKeys::Key4) )
        {
            Channel0.clear();
            Channel1.clear();
        }

        // convert the button below to autorepeat button
        B0.update( 0 != (KEY.get() & MenuKeys::Key5) );
        cli();
        if ( B0.getState() )    LED.set( LED.get() |  0x10 );
        else                    LED.set( LED.get() & ~0x10 );
        sei();

        // do not change this feature - used for grading
        cli();
        ++alive;
        if (25<=alive)
        {
            alive=0;
            LED.set( LED.get() ^ 0x20 );
        }
        sei();

        wdt_reset();                // <--- comment this line, observe and put it back
    }

    return(0);
}
