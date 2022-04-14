//* OOP lib_com_abase.h *
 
#ifndef LIB_COM_ABASE_H_
#define LIB_COM_ABASE_H_
#include <stdint.h>

class com_abase {
public:
// available standard baud rates
const static uint32_t uart_bps_2400   =   2400;
const static uint32_t uart_bps_4800   =   4800;
const static uint32_t uart_bps_9600   =   9600;
const static uint32_t uart_bps_19200  =  19200;
const static uint32_t uart_bps_38400  =  38400;
const static uint32_t uart_bps_57600  =  57600;
const static uint32_t uart_bps_115200 = 115200;

// stop bit settings
const static uint8_t uart_stop_one    = 0;
const static uint8_t uart_stop_two    = 1;

//  parity settings
const static uint8_t uart_parity_none = 0;
const static uint8_t uart_parity_even = 2;
const static uint8_t uart_parity_odd  = 3;

public:
    virtual void initialize( uint32_t baud, uint8_t stop_mode, uint8_t  parity_mode )  = 0;
    virtual void shutdown()     = 0;
    virtual void put( char c )  = 0;
    virtual void flush()        = 0;
    virtual int  get()          = 0;
    virtual uint8_t is_ready()  = 0;

protected:
            com_abase() {} // default constructor is needed since we declare and block copy constructor
private:
            com_abase(const com_abase&);
    void    operator=(const com_abase&);
};


#endif /* LIB_COM_ABASE_H_ */
