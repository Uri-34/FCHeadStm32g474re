#ifndef FC_CAN_HEAD_H
#define FC_CAN_HEAD_H

#include "mbed.h"

class FCCANBus 
    : public mbed::CAN
{
private:

public:
    FCCANBus(PinName rd, PinName td) : mbed::CAN(rd, td) {}

    //! метод сканирование can шины 
    const string detect(); 
};

#endif // FC_CAN_HEAD_H
