#ifndef BUS_H_
#define BUS_H_
#include <libmaple/gpio.h>

class Bus
{
public:
    Bus();
    void Write(unsigned char data);
    uint32_t Read(void);
    void Reset();
};
#endif
