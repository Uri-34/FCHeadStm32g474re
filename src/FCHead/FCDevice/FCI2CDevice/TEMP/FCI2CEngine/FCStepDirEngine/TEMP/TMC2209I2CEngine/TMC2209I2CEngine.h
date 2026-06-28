#ifndef TMC2209UART_ENGINE_H
#define TMC2209UART_ENGINE_H

#include <vector>

#include <mbed.h>

#include "Common/Common.h"
#include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/I2CStepDirEngine.h"

struct TMC2209I2CEngineData
{
    State _state;

    TMC2209I2CEngineData(State &state) : _state{state}
    {}
};

class TMC2209I2CEngine : public I2CStepDirEngine 
{
public:
    // регистры для чтения/записи
    // регистры только для чтения

private:
    uint32_t send(uint8_t address, uint32_t data); // Отправка команды через i2c

public:
    TMC2209I2CEngine(I2CBus *i2c, const string &name, uint8_t address) 
        : I2CStepDirEngine(i2c, name, address)
    {}

    TMC2209I2CEngine(I2CBus *i2c, uint8_t address) 
        : I2CStepDirEngine(i2c, "", address)
    {}

    inline uint32_t stallGuard() 
    { 
        // return send(SGTHRS, data); // ???????????????????????
        return 0; // затычка 
    }

    bool exist() { return true; }

    // void operator=(const TMC2209I2CEngine &engine)
    // {   
    //     *this = engine; 
    // }
};

#endif // TMC2209UART_ENGINE_H