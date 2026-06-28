#include "FCI2CLM75.h"

FCI2CLM75:: FCI2CLM75(FCI2CBus *i2c, const string &name, const uint8_t address)
    : FCI2CDevice(i2c, name, address) 
{}

// bool I2CLM73::exist()
// {
//     // uint8_t cmd;
//     return true; // write(&cmd, 0);
// }

// const FCLM75Data FCI2CLM75::read() 
// {
//     FCLM75Data _data = {0.0}; // данные датчика  
//     if(condition() == Started)
//     {
//         char cmd = 0x00; // Указываем регистр данных температуры
//         char data[2];  
//         // Записываем адрес регистра температуры
//         i2c()->write(address(), &cmd, 1);

//         // Читаем два байта температуры
//         i2c()->read(address(), data, 2);

//         // Объединяем два байта в 16-битное целое число и преобразуем в гр цельсия
//         _data = (((data[0] << 8) | data[1]) >> 1) / 64.0f;   
//     }
//     return _data;
// }
