#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "Head/Device/Device.h"
#include "Bus/I2CBus/I2CBus.h"

class I2CDevice : public Device
{
private:
    I2CBus *_i2c = nullptr; // i2c шина
    uint8_t _address; // адрес датчика

public:
    I2CDevice(I2CBus *i2c, const string &name, uint8_t address) 
        : Device(name),
          _i2c{i2c},
          _address{address} 
    {
        ready() ? set(Ready) : reset(Ready);
    }

    inline I2CBus* i2c() { return _i2c; } 

    inline uint8_t address() { return _address; }

    virtual bool write(const char *data, uint8_t length)
    {
        return (is(Ready) & is(Started)) ? 
            i2c()->write(address() << 1, reinterpret_cast<const char*>(data), length, false) : false;
    }

    virtual bool read(const char *data, uint8_t length)
    {
        return true;
        // return (is(Ready) & is(Started)) ?
        //     _i2c->read((address() << 1) | 1, reinterpret_cast<const char*>(data), length, false) : false; 
    }

    // метод проверяющий на наличие устройства на шине i2c
    virtual bool ready() override 
    {
        bool result = true;
        // дописать фактическиую проверку
            // char data[4];
            // write(data, 4);
            // read(data, 4);

        return result;
    } 
};

#endif // I2C_DEVICE_H