#ifndef FC_PWM_DEVICE_H
#define FC_PWM_DEVICE_H

#include "mbed.h"

#include "FCDevice.h"

class FCPwmDevice 
    : public FCDevice 
{
private:
    PinName _pin; // контакт датчика

public:
    FCPwmDevice(const string &name, const PinName pin) 
        : FCDevice(name), 
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

#endif // FC_PWM_DEVICE_H
