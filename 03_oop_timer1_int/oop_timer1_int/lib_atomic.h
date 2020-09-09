//* lib_semaphore.h *
#ifndef LIB_SEMAPHORE_H_
#define LIB_SEMAPHORE_H_
#include <stdint.h>

class Atomic {
public:
    Atomic() : state(0) {}
    
    void    set()      { state=1; }
    void    clear()    { state=0; }
    uint8_t check()    { return(state); }

private:
    volatile uint8_t state; // volatile keyword is very important in this case
    // without volatile keyword the optimizer will optimize out code to check if the state was changed from within the interrupt
};


#endif /* LIB_SEMAPHORE_H_ */
