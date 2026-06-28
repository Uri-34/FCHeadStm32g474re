#ifndef FC_I2C_BUS_H
#define FC_I2C_BUS_H

#include "mbed.h"

#include "FCState.h"

class FCI2CBus 
    : public mbed::I2C, 
      public FCCondition
{
private:

public:
    FCI2CBus(PinName sda, PinName scl, uint32_t freq) 
        : mbed::I2C(sda, scl)
    {
        frequency(freq);
    }

    //! метод сканирование i2c шины 
    const string scan(); 

    // bool read(uint8_t address, uint8_t buffer[], uint8_t length)
    // {
    //     return mbed::I2C::read() ? ;
    // }

    // bool write(uint8_t address, uint8_t buffer[], uint8_t length)
    // {
    //     return mbed::I2C::write() ? ;
    // }
};

#endif // FC_I2C_BUS_H
