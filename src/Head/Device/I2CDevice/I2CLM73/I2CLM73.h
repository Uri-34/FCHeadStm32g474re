#ifndef I2C_LM73_H
#define I2C_LM73_H

/*!
	\brief Класс для организации работы датчика температуры LM73 на I2C шине 
*/
#include "Head/Device/I2CDevice/I2CDevice.h"

struct LM73Data
{
    float _temperature;

    LM73Data(float temperature = 0.0)
        : _temperature{temperature}
    {}

    void operator=(float temperature)
    {
        _temperature = temperature;
    }
};

class I2CLM73 : public I2CDevice
{
private:
    // ==== Регистры ====
    // ==== Калибровочные регистры ====
    // ==== Калибровочные коэффициенты ====

    // -------------------------------------

    const string ErrorExist = " ошибка доступа к датчику...";
    
public:
    I2CLM73(I2CBus *i2c, const string &name, const uint8_t address);
    virtual ~I2CLM73() {};

    // bool exist() override final;

    const LM73Data read();
};

#endif // I2C_LM73_H