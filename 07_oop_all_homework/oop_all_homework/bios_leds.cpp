//* OOP bios_leds.c *
#include "bios_leds.h"
#include <avr/io.h>

uint8_t LEDs::use_count = 0;

LEDs::LEDs (void)
{
    if (use_count==0)
    {
        // initialize LED output pins on lower 6 bits of Port B
        DDRB  = DDRB  | 0b00111111;   // affect DDR only the lower pins - set to push-pull out/1
        // PORTB = PORTB | 0x0001111111;   // affect only the 6 output pins by setting them to 1
        PORTB = PORTB & 0b11000000;   // affect only the 6 output pins by setting them to 0
    }
    ++use_count;
}

LEDs::~LEDs (void)
{
    --use_count;
    if (use_count==0)
    {
        // to be safe and low power set the upper nibble back to input and shut off pull up resistors
        DDRB  &= 0b11000000;
        PORTB &= 0b11000000;
    }
}

uint8_t LEDs::get (void) const
{
    // no need for delay after writing to PORTB
    // as it takes time to call the function
    return( PINB & 0b00111111 );
}

void LEDs::set ( uint8_t val )
{
    PORTB = (PORTB & 0b11000000) | (val & 0b00111111);
}
