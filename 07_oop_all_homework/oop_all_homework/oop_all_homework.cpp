/*
 * OOP Case study: all features test
 *
 * Created: 9/9/2020 1:27:00 AM
 * Updated: 12/14/2020 4:50:00 PM
 * Updated: 11/29/2021 11:21:00 PM
 * Updated: 12/14/2021 11:43:00 PM (reduced suggested buffer size)
 * Updated: 12/4/2021  8:39:00 PM (autorepeat removed button test)
 * Updated: 12/4/2021  8:40:00 PM (separated declaration of circular buffers for channels 0 and 1 to allow for different timings)
 * Author: Aleksander Malinowski
 */

#include "lib_atomic.h"
#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_timer1.h"
#include "bios_adc.h"
#include "lib_pwm.h"

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
    uint8_t alive = 0;

    PWM_Generator<4, uint8_t> PWM(100, 100);

    myInterrupt myInt(10000/50, &PWM, &LED);

    Timer1::getInstance().initialize(10000, &myInt, Timer1::timer_prescale_1);

    ADConverter &V1 = ADConverter::getInstance();
    V1.initialize(ADConverter::CH0, ADConverter::CHMAX);

// TO DO
    CircularBuffer<uint8_t, uint8_t, uint8_t, 1, 1> Channel0;
    //             ^^ replace with the correct data type for data storage
    //                      ^^ replace with the correct data type for the long average sum of data to be averaged
    //                               ^^ replace with the correct data type for the size used with this buffer
    //                                        ^^     compute 50Hz x ?? seconds and set the buffer size/long average capacity
    //                                           ^^  compute 50Hz x  ? seconds and set the short average capacity

// TO DO
    CircularBuffer<uint8_t, uint8_t, uint8_t, 1, 1> Channel1;
    //             ^^ replace with the correct data type for data storage
    //                      ^^ replace with the correct data type for the long average sum of data to be averaged
    //                               ^^ replace with the correct data type for the size used with this buffer
    //                                        ^^     compute 50Hz x ?? seconds and set the buffer size/long average capacity
    //                                           ^^  compute 50Hz x  ? seconds and set the short average capacity

    sei();
    // MAIN SUPERLOOP
    while(1)
    {
        while ( !V1.isReady() ) ;
        V1.resetReady();

        uint8_t adc_ch0  = V1.getRecent(ADConverter::CH0);
        uint8_t adc_ch1  = V1.getRecent(ADConverter::CH1);

// TO DO
// Uncomment #define below if your circular buffer works
// #define _CIRCULAR_BUFFER_WORKS 1

#ifndef _CIRCULAR_BUFFER_WORKS
        PWM.setPWM(0, static_cast<uint16_t>(100U)*(adc_ch0)/255U);
        PWM.setPWM(1, static_cast<uint16_t>(100U)*(256U-adc_ch0)/255U);
        PWM.setPWM(2, static_cast<uint16_t>(100U)*(adc_ch1)/255U);
        PWM.setPWM(3, static_cast<uint16_t>(100U)*(256U-adc_ch1)/255U);
#else
        Channel0.store( adc_ch0 );
        Channel1.store( adc_ch1 );

        // if not buffer ready keep averages at 0
        // otherwise, compute the averages using two
        // custom circular buffer templates of <uint8_t>

        if ( Channel0.ready() )
        {
            PWM.setPWM(0, static_cast<uint16_t>(100U)*Channel0.ShortAverage()/255U);
            PWM.setPWM(1, static_cast<uint16_t>(100U)*Channel0.LongAverage()/255U);
        } else {
            PWM.setPWM(0, 0);
            PWM.setPWM(1, 0);
        }
        if ( Channel1.ready() )
        {
            PWM.setPWM(2, static_cast<uint16_t>(100U)*Channel1.ShortAverage()/255U);
            PWM.setPWM(3, static_cast<uint16_t>(100U)*Channel1.LongAverage()/255U);
        } else {
            PWM.setPWM(2, 0);
            PWM.setPWM(3, 0);
        }
#endif


        if ( 0 != (KEY.get() & MenuKeys::Key4) )
        {
            Channel0.clear();
        }

        if ( 0 != (KEY.get() & MenuKeys::Key5) )
        {
            Channel1.clear();
        }

        // do not change this feature - used for grading
        cli();
        ++alive;
        if (25<=alive)
        {
            alive=0;
            LED.set( LED.get() ^ LEDs::LED5 );
        }
        sei();

        wdt_reset();
    }

    return(0);
}
