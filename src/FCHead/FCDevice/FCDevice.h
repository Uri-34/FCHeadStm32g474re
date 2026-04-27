#ifndef FC_DEVICE_H
#define FC_DEVICE_H

#include "mbed.h"

#include "FCStateObject.h"

class FCDevice 
    : public FCStateObject 
{
private:
    const std::chrono::milliseconds DEFAULT_DEVICE_TIME_OUT = 500ms;

    // период опроса устройства
    std::chrono::milliseconds _timeOut; 

public:
    FCDevice(const string &name) 
        : FCStateObject{name},
        _timeOut{DEFAULT_DEVICE_TIME_OUT}
    {}

    // устанавливаем период опроса устройства
    inline void setTimeOut(std::chrono::milliseconds timeOut) { _timeOut = timeOut; }
    
    // возвращаем период опроса устройства
    inline std::chrono::milliseconds timeOut() { return _timeOut; }

    // виртуальный метод проверяющий на наличие устройства на шине
    virtual bool ready() { return true; }
};

#endif // FC_DEVICE_H
