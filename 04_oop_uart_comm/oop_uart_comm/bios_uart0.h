//* OOP bios_uart0.h *
#ifndef UART0_H_
#define UART0_H_

#include "lib_com_abase.h"

class Uart0 : public com_abase {
public:
    static Uart0& getInstance() { return(instance); }

    void initialize( uint32_t baud, uint8_t stop_mode, uint8_t  parity_mode );
    void shutdown();

    void put( char c );
    void flush();
    int  get();
    uint8_t is_ready();

private:
    static Uart0 instance;
    Uart0() {}
    Uart0(const Uart0&);
    void operator=(const Uart0&);
};

#endif /* UART0_H_ */
