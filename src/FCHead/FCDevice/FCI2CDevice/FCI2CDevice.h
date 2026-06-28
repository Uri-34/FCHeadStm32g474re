#ifndef FC_I2C_DEVICE_H
#define FC_I2C_DEVICE_H

#include "FCDevice.h"
#include "FCI2CBus.h"

class FCI2CDevice 
    : public FCDevice
{
private:
    FCI2CBus *_i2c = nullptr; // i2c шина
    uint8_t _address; // адрес датчика

protected:    
    virtual bool init() override 
    { 
        bool result = true;

        // реализовать проверку присутствия устройства на шине
        // --- опросить устройство на шине _i2c по адресу _address
        // --- если отвечает - result = true иначе - result = false 

        return result;
    }

    inline bool write(const char *data, uint8_t length)
    {
        return is(FCState::Ready) ? _i2c->write(_address << 1, reinterpret_cast<const char*>(data), length, false) : false;
    }

    bool read(const char *data, uint8_t length)
    {
        return true; // is(FCReadyState::Ready) ? _i2c->read((_address << 1) | 1), reinterpret_cast<const char*>(data), length, false) : false; 
    }

public:
    FCI2CDevice(FCI2CBus *i2c,const string &name, uint8_t address) 
        : FCDevice(name),
          _i2c{i2c},
          _address{address} 
    {
        set(init() ? FCState::Ready : FCState::NotReady);
    }
};

#endif // FC_I2C_DEVICE_H