#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include "Head/Device/Device.h"

class CANDevice : public Device
{
private:
    CAN *_can = nullptr; // can шина
    int _address; // адрес датчика

public:
    CANDevice(CAN *can, const string &name, uint8_t address) 
        : Device(name),
          _can{can},
          _address{address} 
    {
        if(exist())
        {
            set(Started);
        }
    }

    bool exist() override final
    {
        // реализовать проверку на работоспособность 
    }

    inline CAN* can() { return _can; } 

    inline uint8_t address() { return _address; }

    inline bool write(CANMessage &msg)
    {
        return _can->write(msg);
    }

    inline bool read(CANMessage &msg, int handle)
    {
        return _can->read(msg, handle);
    }
};

#endif // CAN_DEVICE_H