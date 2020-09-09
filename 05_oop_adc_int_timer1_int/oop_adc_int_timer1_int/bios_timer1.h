//* OOP bios_timer1.h *
#ifndef TIMER1_H_
#define TIMER1_H_
#include <stdint.h>

#include "lib_int_abase.h"

class Timer1 {
public:
    static const uint8_t timer_prescale_1    = 1;
    static const uint8_t timer_prescale_8    = 2;
    static const uint8_t timer_prescale_64   = 3;
    static const uint8_t timer_prescale_256  = 4;
    static const uint8_t timer_prescale_1024 = 5;

public:
    static Timer1&  getInstance()   { return(instance);  }

    void initialize( uint32_t frequency, int_abase* int_handler_class, uint8_t prescaler );
    void shutdown();
    
    int_abase* getHandler() const { return(interrupt_handler_class); }
    
private:
    static Timer1       instance;
           int_abase*   interrupt_handler_class;
    
    Timer1() : interrupt_handler_class(0) {}
    Timer1(const Timer1&);
    void operator=(const Timer1&);
};

#endif /* TIMER1_H_ */
