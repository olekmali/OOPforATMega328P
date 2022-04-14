//* OOP lib_com.h *
#ifndef LIB_COM_H_
#define LIB_COM_H_

#include <stddef.h>     // needed for size_t
#include <stdint.h>     // needed for int8_t
#include "lib_com_abase.h"

class comlink {
public:
    comlink(com_abase * comdevice) : device(comdevice)  {}
    ~comlink()          { shutdown(); }

    void shutdown()     { device->shutdown();           }
    void put( char c )  { device->put(c);               }
    void flush()        { device->flush();              }
    int  get()          { return( device->get() );      }
    uint8_t is_ready()  { return(  device->is_ready() );}

protected:
    com_abase * device;
};

class textlink : public comlink {
public:
    textlink(com_abase * comdevice) : comlink(comdevice)  {}
    void puts(const char* text);
    void gets(      char* text, size_t max_size);
};

class console : public comlink {
    public:
    console(com_abase * comdevice) : comlink(comdevice)  {}
    void puts(const char* text);
    void edit(      char* text, size_t max_size); // with edit capabilities and echo
};

class binarylink : public comlink {
    public:
    binarylink(com_abase * comdevice) : comlink(comdevice)  {}
    void write(const char* buffer, size_t length);
    void read (      char* buffer, size_t length);
};



#endif /* LIB_COM_H_ */
