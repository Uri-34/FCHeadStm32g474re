#ifndef FC_I2C_LM75_H
#define FC_I2C_LM75_H

/*!
	\brief Класс для организации работы датчика температуры LM73 на I2C шине 
*/
#include "FCI2CDevice.h"

class FCI2CLM75 
    : public FCI2CDevice
{
public:
    FCI2CLM75(FCI2CBus *i2c, const string &name, const uint8_t address);

    float temperature() 
    { 
        float result = 0;
        read((char*)&result, sizeof(result));
        return result; 
    };

protected:

private:
    // ==== Регистры ====
    // ==== Калибровочные регистры ====
    // ==== Калибровочные коэффициенты ====

    float _temperature;

    // -------------------------------------
};

#endif // FC_I2C_LM75_H