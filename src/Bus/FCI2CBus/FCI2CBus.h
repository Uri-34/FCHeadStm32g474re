#ifndef FC_I2C_BUS_H
#define FC_I2C_BUS_H

#include "mbed.h"

class FCI2CBus 
    : public mbed::I2C
{
private:

public:
    FCI2CBus(PinName sda, PinName scl) : mbed::I2C(sda, scl) {}

    //! метод сканирование i2c шины 
    const string detect(); 
};

#endif // FC_I2C_BUS_H
