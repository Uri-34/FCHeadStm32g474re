#ifndef I2C_ENGINE_H
#define I2C_ENGINE_H

#include "Pins.h"
#include "Head/Device/I2CDevice/I2CDevice.h"

class I2CEngine : public I2CDevice 
{
public:
    I2CEngine(I2CBus *i2c, const string &name, uint8_t address) 
        : I2CDevice(i2c, name, address)
    {}
};

#endif // I2C_ENGINE_H