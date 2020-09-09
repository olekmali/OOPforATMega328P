//* OOP bios_timer1.cpp *
#include "bios_timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

Timer1 Timer1::instance;

void Timer1::initialize( uint32_t frequency, int_abase* int_handler_class, uint8_t prescaler )
{
                                     // -  1  8 64 256 1024
    const static uint8_t prescales[] = {0, 0, 3, 6, 8, 10};

    interrupt_handler_class = int_handler_class;

    TCNT1=0x00;
    // set timer0 counter initial value to 0
    // (note: entirely redundant as it was never used before)

    OCR1A = (16000000L>>prescales[prescaler]) / frequency - 1;

    TCCR1A = 0x00;
    // AABBCCMM - lower two bits of 4-bit mode MMMM

    TCCR1B = 0x08 | (0x07 & prescaler);
    // ??-MMPPP - MM is upper two bits of mode MMMM, PPP is clock rescale
    // where PPP of 011 is clock rescale of 64
    // where MM MM is the Timer mode, we use 01 00 - CTC

    TIFR1  &= 0;
    // reset any pending Timer1 interrupts
    // (note: entirely redundant as none is pending - it was never used before)

    TIMSK1 = (TIMSK1 & ~(1<<ICIE1|1<<OCIE1B|1<<TOIE1) ) | (1<<OCIE1A);
    // Preserve other Timer interrupt settings, just set applicable to Timer1
    //      OCIE1A  - enable Timer1 match OCR1A match interrupt
    //      TOIE1   - disable Timer1 overflow interrupt (note: redundant as it was already disabled at power on)
}

void Timer1::shutdown()
{
    TIMSK1 &= 0;
    // disable all Timer1 interrupts
    // which is the same effort as disabling just one interrupt but can be reused in other examples

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIFR1  = 0;
    TIMSK1 = 0;
}


ISR(TIMER1_COMPA_vect)
{
    Timer1::getInstance().getHandler()->handle();
}

