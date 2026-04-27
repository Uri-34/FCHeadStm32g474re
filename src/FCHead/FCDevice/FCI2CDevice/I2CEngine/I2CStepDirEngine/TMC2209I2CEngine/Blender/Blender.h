#ifndef BLENDER_H
#define BLENDER_H

#include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/TMC2209I2CEngine/TMC2209I2CEngine.h"

class Blender : public TMC2209I2CEngine 
{
public:
    Blender(I2CBus *i2c = nullptr, uint8_t address = 0x00) 
        : TMC2209I2CEngine(i2c, "blender", address)
    {}
};

#endif // BLENDER_H