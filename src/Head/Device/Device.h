#ifndef DEVICE_H
#define DEVICE_H

#include "mbed.h"

#include "Object/Object.h"

class Device : public Object 
{
private:
    const std::chrono::milliseconds DEFAULT_DEVICE_TIME_OUT = 500ms;

    // период опроса устройства
    std::chrono::milliseconds _timeOut; 

public:
    Device(const string &name) 
        : Object{name},
        _timeOut{DEFAULT_DEVICE_TIME_OUT}
    {}

    // устанавливаем период опроса устройства
    inline void setTimeOut(std::chrono::milliseconds timeOut) { _timeOut = timeOut; }
    
    // возвращаем период опроса устройства
    inline std::chrono::milliseconds timeOut() { return _timeOut; }

    // виртуальный метод проверяющий на наличие устройства на шине
    virtual bool ready() { return true; }
};

#endif // DEVICE_H