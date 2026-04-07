#ifndef VL53L3CX_SENSOR_H
#define VL53L3CX_SENSOR_H

#include "Pins.h"
#include "Head/Device/I2CDevice/I2CDevice.h"

struct VL53L3CXI2CData
{
    uint16_t _distance;

    VL53L3CXI2CData(uint16_t distance)
        : _distance{distance} 
    {}
};

class VL53L3CXI2C : public I2CDevice
{
private:
    // Регистры датчика (минимально необходимые)
    const char VL53L3CX_REG_WHO_AM_I      = 0x01;
    const char VL53L3CX_REG_MEASURE_START = 0x00;
    const char VL53L3CX_REG_DATA_READY    = 0x1F;
    const char VL53L3CX_REG_DISTANCE      = 0x12;

    const char VL53L3CX_AM_I              = 0xE1;

public:
    VL53L3CXI2C(I2CBus *i2c, const string &name, uint8_t address) 
        : I2CDevice(i2c, name, address)
    {};

    bool ready(); 

    bool readRegister(uint8_t reg, uint8_t *data, int len); // Функция чтения регистра 
    bool readDistance(uint16_t *distance_mm); // чтение двухбайтового значения (расстояния)
};

#endif // VL53L3CX_SENSOR_H