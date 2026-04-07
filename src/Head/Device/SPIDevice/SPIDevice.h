#ifndef SPI_DEVICE_H
#define SPI_DEVICE_H

#include "Head/Device/Device.h"
#include "Bus/SPIBus/SPIBus.h"

class SPIDevice : public Device
{
private:
    SPIBus *_spi = nullptr; // i2c шина
    int _address; // адрес датчика

public:
    SPIDevice(SPIBus *spi, const string &name, uint8_t address) 
        : Device(name),
          _spi{spi},
          _address{address} 
    {
        if(exist())
        {
            set(Started);
        }
    }

    inline SPIBus* i2c() { return _spi; } 

    inline uint8_t address() { return _address; }

    virtual bool write(const char *data, uint8_t length)
    {
        // переписать правильно для spi шины (это копия класса I2CBus)
        // return _spi->write(address() << 1, reinterpret_cast<const char*>(data), length, false);
    }

    virtual bool read(const char *data, uint8_t length)
    {
        // return _i2c->read((address() << 1) | 1, reinterpret_cast<const char*>(data), length, false);
    }
};

#endif // I2C_DEVICE_H