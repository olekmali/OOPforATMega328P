//* OOP Interrupt Wrapper Abstract Base Class *
#ifndef LIB_INT_ABASE_H_
#define LIB_INT_ABASE_H_
#include <stdint.h>

class int_abase {
public:
            int_abase() {} // needed since we declare and block copy constructor
    virtual void handle()           = 0;

private:
            int_abase(const int_abase&);
    void    operator=(const int_abase&);
};

#endif /* LIB_INT_ABASE_H_ */
