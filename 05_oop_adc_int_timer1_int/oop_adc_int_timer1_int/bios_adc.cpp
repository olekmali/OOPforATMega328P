//* OOP bios_adc.cpp *
#include "bios_adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ADConverter ADConverter::instance;

// It would be too much effort and inefficiency to pack these two variables into the ADConverter class
static volatile uint8_t latest_results[ADConverter::CHMAX];
static volatile uint8_t channel;        //

void ADConverter::initialize() {
    channel = 0;
    new_results = 0;

    ADCSRA  |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1<<ADIE);  // Set ADConverter clock to 125kHz (==16MHz/128), enable ADConverter complete interrupt
    ADMUX    = 0;                   // for now let us use the very 1st channel
    ADMUX   |= (1 << REFS0);        // use VCC as Vref         (==AVCC==5V)
    ADMUX   |= (1 << ADLAR);        // left justify the result so that 8bits can be read from the high register
    // this works well if you do not need to have a 4bit cushion to prevent overflow on computations
    
    ADCSRA  |= (1 << ADEN);         // start ADConverter
}

void ADConverter::shutdown() {
    ADCSRA = 0;
    ADMUX = 0;
}

void ADConverter::startRound  (void)
{
    //  ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel);
    ADMUX = ( ADMUX & 0xE0 );
    channel = 0;
    ADCSRA |= (1 << ADSC);  // start conversion
}

ISR(ADC_vect)
{
    latest_results[channel] = ADCH;
        channel++;
    if( channel == (ADConverter::CHMAX) )
    {
        ADConverter::getInstance().setReady();
    } else {
        ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel);
        ADCSRA |= (1 << ADSC);  // start conversion
    }
}

uint8_t ADConverter::getRecent(uint8_t channel) const
{
    uint8_t value;
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    value = latest_results[channel];
    SREG = sreg_save;                   // restore interrupts
    return(value);
}

void ADConverter::setRange(uint8_t channels) {
    if (total_channels<=ADConverter::CHMAX)
    total_channels=channels;
    else
    total_channels=ADConverter::CHMAX;
}
