#ifndef I2C_PIEZO_PUMP_H
#define I2C_PIEZO_PUMP_H

#include "Head/Device/I2CDevice/I2CDevice.h"

class I2CPiezoPump : public I2CDevice
{
private:
    const string ErrorExists = "насос не найден...";

public:
    I2CPiezoPump(I2CBus *i2c, const string &name, const uint8_t address) 
        : I2CDevice(i2c, name, address) 
    {}

   ~I2CPiezoPump() 
   {}

    bool ready() override final
    {
        // в дальнейшем доделать и отладить проверку наличия насоса
        return true;
    }

    //! сделать один качек
    void pitch();

    //! сделать number качков
    void pitches(int number)
    {
        while(number > 0)
        {
            pitch();
            number--;
        }
    }
};

#endif // I2C_PIEZO_PUMP_H