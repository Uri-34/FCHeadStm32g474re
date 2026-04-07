#ifndef UART_DEVICE_H
#define UART_DEVICE_H

#include "Head/Device/Device.h"

class UARTDevice : public Device
{
private:
    uint8_t _address;
    
public:
    UARTDevice(const string &name, const PinName &pin, uint8_t address) 
        : Device(name),
          _address{address}
    {}

    uint8_t address() { return _address; }

    inline bool write(uint8_t *data, int length)
    {
        return true;
    }

    inline bool read(uint8_t *data, int length)
    {
        return true;
    }

    bool ready() override
    {
        // написать проверку на наличие ШИМ устройства
        // но сначала разобраться как это сделать !!!!
        return true;
    }
};

#endif // UART_DEVICE_H