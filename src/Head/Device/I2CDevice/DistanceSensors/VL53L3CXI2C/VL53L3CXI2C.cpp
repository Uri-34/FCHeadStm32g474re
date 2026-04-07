#include "Head/Device/I2CDevice/DistanceSensors/VL53L3CXI2C/VL53L3CXI2C.h"

// void VL53L3CXI2C::start()
// {
//     char cmd[] = {VL53L3CX_REG_MEASURE_START, 0x01}; // запуск одиночного измерения
//     i2c()->write(address() << 1, cmd, 2);
// }

bool VL53L3CXI2C::readRegister(uint8_t reg, uint8_t *data, int len) 
{
    char tx[1] = {reg};
    if(i2c()->write(address() << 1, tx, 1)) 
    {
        return false;
    }
    if(i2c()->read((address() << 1) | 1, reinterpret_cast<char*>(data), len)) 
    {
        return false;
    }
    return true;
}

bool VL53L3CXI2C::ready() 
{
    uint8_t status;
    if(!readRegister(VL53L3CX_REG_DATA_READY, &status, 1)) 
    {
        return false;
    }
    return (status & 0x01);
}

bool VL53L3CXI2C::readDistance(uint16_t *distance_mm) 
{
    uint8_t data[2];
    if (!readRegister(VL53L3CX_REG_DISTANCE, data, 2)) 
    {
        return false;
    }
    *distance_mm = static_cast<uint16_t>((data[0] << 8) | data[1]);
    return true;
}
