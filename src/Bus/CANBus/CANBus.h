#ifndef CAN_HEAD_H
#define CAN_HEAD_H

#include "mbed.h"

class CANBus 
    : public mbed::CAN
{
private:

public:
    CANBus(PinName rd, PinName td) : mbed::CAN(rd, td) {}

    //! метод сканирование can шины 
    const string detect(); 
};

#endif // CAN_HEAD_H