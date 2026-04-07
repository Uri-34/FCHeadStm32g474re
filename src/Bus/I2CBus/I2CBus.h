#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "mbed.h"

class I2CBus : public mbed::I2C
{
private:

public:
    I2CBus(PinName sda, PinName scl) : mbed::I2C(sda, scl) {}

    //! метод сканирование i2c шины 
    const string detect(); 
};

#endif // I2C_BUS_H