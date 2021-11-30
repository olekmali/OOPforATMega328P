/*
 * OOP Case study: oop_adc_int_timer1_int_pwm
 *
 * Created: 9/9/2020 1:27:00 AM
 * Updated: 12/2/2020 1:12:00 AM
 * Updated: 11/29/2021 11:13:00 PM
 *  Author: Aleksander Malinowski
 */

#include "lib_atomic.h"
#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_timer1.h"
#include "bios_adc.h"
#include "lib_pwm.h"


#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>


class myInterrupt : public int_abase {
public:
    virtual void    handle();
    myInterrupt(uint16_t ADC_interval, PWM_Generator<2, uint8_t>* GEN, LEDs* PWM_Pins)
        : counter(0), interval(ADC_interval), PWM_GEN(GEN), PWM_PIN(PWM_Pins) {}
private:
    uint16_t                    counter;
    uint16_t                    interval;
    PWM_Generator<2, uint8_t>*  PWM_GEN;
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
    wdt_enable(3);              // change it to 0 or 1 - less than 50ms, observe and put it back
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

    PWM_Generator<2, uint8_t> PWM(100, 100);

    myInterrupt myInt(10000/50, &PWM, &LED);

    Timer1::getInstance().initialize(10000, &myInt, Timer1::timer_prescale_1);

    ADConverter &V1 = ADConverter::getInstance();
    V1.initialize(ADConverter::CH0, ADConverter::CH2);

    sei();
    // MAIN SUPERLOOP
    while(1)
    {
        while ( !V1.isReady() ) ;
        V1.resetReady();

        PWM.setPWM(0, static_cast<uint16_t>(100)*V1.getRecent(ADConverter::CH0)/255);
        PWM.setPWM(1, static_cast<uint16_t>(100)*V1.getRecent(ADConverter::CH1)/255);

        wdt_reset();                // comment this line, observe and put it back
    }

    return(0);
}
