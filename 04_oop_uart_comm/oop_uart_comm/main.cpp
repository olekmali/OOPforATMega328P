/*
 * OOP Case study: oop_uart_comm
 *
 * Created: 9/9/2020 1:27:00 AM
 *  Author: Aleksander Malinowski
 */


#include "bios_keys.h"
#include "bios_leds.h"
#include "bios_uart0.h"
#include "lib_com.h"
#include <stdlib.h>

uint8_t count_bits(uint8_t byte)
{
    uint8_t result=0;
    while ( byte != 0 )
    {
        if ( (byte & 0x01) != 0 ) ++result;
        byte = byte >> 1;
    }
    return(result);
}


int main(void)
{
    // INITIALIZATION
    MenuKeys K;
    LEDs     L;

    Uart0    &com0 = Uart0::getInstance();
    com0.initialize(com_abase::uart_bps_9600, com_abase::uart_stop_two, com_abase::uart_parity_none);
    textlink datalink( &com0 );

    // MAIN SUPERLOOP
    while(1)
    {
        char buffer[5];
        uint8_t local = K.get();

        itoa( count_bits(local), buffer, 10);
        datalink.puts(buffer);
        datalink.put('\n');
        datalink.put('\r');

        if ( datalink.is_ready() ) {
            uint8_t remote = datalink.get();
            L.set( remote-' ' );
        }
    }

    return(0);
}