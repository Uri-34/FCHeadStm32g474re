#ifndef PWM_DEVICE_H
#define PWM_DEVICE_H

#include "mbed.h"

#include "Head/Device/Device.h"

class PwmDevice : public Device 
{
private:
    PinName _pin; // контакт датчика

public:
    PwmDevice(const string &name, const PinName pin) 
        : Device(name), 
          _pin{pin} 
    {
        ready() ? set(Ready) : reset(Ready);
    };

    virtual bool ready() override
    {
        // написать проверку на наличие ШИМ устройства
        // но сначала разобраться как это сделать !!!!
        return true;
    }
};

#endif // PWM_DEVICE_H