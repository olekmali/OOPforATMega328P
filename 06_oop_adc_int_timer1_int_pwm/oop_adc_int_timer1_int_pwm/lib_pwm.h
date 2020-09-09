//* lib_pwm.h *
#ifndef LIB_PWM_H_
#define LIB_PWM_H_
#include <stdint.h>
#include <avr/interrupt.h>

template<uint8_t PWM_NCHANNELS, typename pwmcnt_t>
class PWM_Generator {
public:
    PWM_Generator(/* TBA: PortSelection, */ pwmcnt_t resolution = 100, pwmcnt_t percent100value = 100 );

    void        setPWM(uint8_t channel, pwmcnt_t percentage);
    pwmcnt_t    getPWM(uint8_t channel) const;

    uint8_t     NextStepUpdate(uint8_t previous);

private:
    pwmcnt_t MyPWMCoef_params[PWM_NCHANNELS];
    pwmcnt_t MyPWMCoef_reload[PWM_NCHANNELS];

    bool     MyPWMCoef_change;
    pwmcnt_t PWM_RESOLUTION;
    pwmcnt_t PWM_100_PERCENT;
    uint8_t  PWM_CH_MASK;

    pwmcnt_t counter;
private:
    PWM_Generator(const PWM_Generator&);
    void operator=(const PWM_Generator&);
};



template<uint8_t PWM_NCHANNELS, typename pwmcnt_t>
PWM_Generator<PWM_NCHANNELS, pwmcnt_t>::PWM_Generator(/* TBA: PortSelection, */ pwmcnt_t resolution, pwmcnt_t percent100value )
:   MyPWMCoef_change(false), PWM_RESOLUTION(resolution), PWM_100_PERCENT(percent100value), PWM_CH_MASK(0), counter(0)
{
    for(uint8_t i=0; i<PWM_NCHANNELS; ++i)
    {
        PWM_CH_MASK = (PWM_CH_MASK<<1) | 0x01;
        MyPWMCoef_params[i] = 0;
        MyPWMCoef_reload[i] = 0;
    }
}

template<uint8_t PWM_NCHANNELS, typename pwmcnt_t>
void PWM_Generator<PWM_NCHANNELS, pwmcnt_t>::setPWM(uint8_t channel, pwmcnt_t percentage)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();

    if ( PWM_100_PERCENT < percentage)
    percentage = PWM_100_PERCENT;

    if ( PWM_NCHANNELS>channel )
    {
        MyPWMCoef_reload[channel] = ( static_cast<uint16_t>(percentage) * PWM_RESOLUTION ) / PWM_100_PERCENT;
        MyPWMCoef_change = true;
        // Note: Actual change in PWM will take effect when a new PWM cycle starts to prevent glitches
    }

    SREG = sreg_save;                   // restore interrupts
}

template<uint8_t PWM_NCHANNELS, typename pwmcnt_t>
pwmcnt_t PWM_Generator<PWM_NCHANNELS, pwmcnt_t>::getPWM(uint8_t channel) const
{
    pwmcnt_t value;
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();

    if ( PWM_NCHANNELS>channel )
    value = MyPWMCoef_reload[channel];
    else
    value = 0;

    SREG = sreg_save;                   // restore interrupts
    return(value);
}

template<uint8_t PWM_NCHANNELS, typename pwmcnt_t>
uint8_t PWM_Generator<PWM_NCHANNELS, pwmcnt_t>::NextStepUpdate(uint8_t pwmbyte)
{
    // PWM software counter update
    if ( PWM_RESOLUTION <= counter )
    {
        counter = 0;
        if ( MyPWMCoef_change )
        {
            for (uint8_t i=0; i<PWM_NCHANNELS; ++i)
            MyPWMCoef_params[i] = MyPWMCoef_reload[i];
            MyPWMCoef_change = 0;
            // Note: Actual change in PWM will take effect when a new PWM cycle starts
        }
    }
    else
    {
        counter++;
    }

    // PWM output computation based on software counter
    pwmbyte = pwmbyte & ~PWM_CH_MASK;

    uint8_t current = 0x01;
    for (uint8_t i=0; i<PWM_NCHANNELS; ++i)
    {
        if ( counter < MyPWMCoef_params[i] ) pwmbyte |= current;
        // else do not set that bit as ON

        current <<= 1;
    }

    return(pwmbyte);
}


#endif /* LIB_PWM_H_ */