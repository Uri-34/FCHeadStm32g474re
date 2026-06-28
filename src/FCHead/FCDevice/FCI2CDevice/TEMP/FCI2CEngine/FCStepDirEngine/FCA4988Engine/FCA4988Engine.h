#ifndef FC_A4988_ENGINE_H
#define FC_A4988_ENGINE_H

#include <vector>

#include <mbed.h>

#include "FCCommon.h"
#include "FCStepDirEngine.h"

struct FCA4988Data
{

    FCA4988Data()
    {}
};

class FCA4988Engine 
    : public FCStepDirEngine 
{
public:
    // регистры для чтения/записи
    // регистры только для чтения

private:
    uint32_t send(uint8_t address, uint32_t data); // Отправка команды через i2c

public:
    FCTMC2209I2CEngine(const string &name, uint8_t address) 
        : FCStepDirEngine(name, address)
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

#endif // FC_A4988_ENGINE
