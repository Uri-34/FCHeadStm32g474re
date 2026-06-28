#ifndef PERELSTATIC_PUMP_H
#define PERELSTATIC_PUMP_H

#include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/TMC2209I2CEngine/TMC2209I2CEngine.h"

class PerelstaticPump : public TMC2209I2CEngine 
{
public:
    PerelstaticPump(I2CBus *i2c = nullptr, uint8_t address = 0x00) 
        : TMC2209I2CEngine(i2c, "perelstaticPump", address)
    {}
};

#endif // PERELSTATIC_PUMP_H