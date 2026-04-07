#include "Head/Device/I2CDevice/I2CLM73/I2CLM73.h"

I2CLM73:: I2CLM73(I2CBus *i2c, const string &name, const uint8_t address)
    : I2CDevice(i2c, name, address) 
{}

// bool I2CLM73::exist()
// {
//     // uint8_t cmd;
//     return true; // write(&cmd, 0);
// }

const LM73Data I2CLM73::read() 
{
    LM73Data _data = {0.0}; // данные датчика  
    if(condition() == Started)
    {
        char cmd = 0x00; // Указываем регистр данных температуры
        char data[2];  
        // Записываем адрес регистра температуры
        i2c()->write(address(), &cmd, 1);

        // Читаем два байта температуры
        i2c()->read(address(), data, 2);

        // Объединяем два байта в 16-битное целое число и преобразуем в гр цельсия
        _data = (((data[0] << 8) | data[1]) >> 1) / 64.0f;   
    }
    return _data;
}
