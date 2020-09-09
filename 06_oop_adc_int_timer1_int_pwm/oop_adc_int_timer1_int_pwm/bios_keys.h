//* OOP bios_keys.h *
#ifndef BIOS_KEYS_H_
#define BIOS_KEYS_H_
#include <stdint.h>

class MenuKeys {
public:
    const static uint8_t Key7 = 0x08;
    const static uint8_t Key6 = 0x04;
    const static uint8_t Key5 = 0x02;
    const static uint8_t Key4 = 0x01;

public:
    MenuKeys();
    ~MenuKeys();
    uint8_t get() const;

private:
    MenuKeys(const MenuKeys&);          // no passing by value please
    void operator=(const MenuKeys&);    // no reassignment

private:
    static uint8_t use_count;           // in case multiple instances are declared
};


#endif /* BIOS_KEYS_H_ */
