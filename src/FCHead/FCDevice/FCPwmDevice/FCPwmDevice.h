#ifndef FC_PWM_DEVICE_H
#define FC_PWM_DEVICE_H

#include "mbed.h"

#include "FCCommon.h"
#include "FCDevice.h"

class FCPwmDevice 
    : public FCDevice 
{
private:
    // контакт
    PinName _pin; 

    // частота для генерации ШИМ сигнала
    uint _frequency = 0;

protected:    
    virtual bool init() override = 0;

public:
    FCPwmDevice(const string &name, const PinName pin) 
        : FCDevice(name), 
          _pin{pin} 
    {};

    inline void setFrequency(uint frequency) { _frequency =  frequency; }
    inline uint Frequency() { return _frequency; }

};

#endif // FC_PWM_DEVICE_H
