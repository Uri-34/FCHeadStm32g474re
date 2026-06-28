#ifndef FC_CAN_HEAD_H
#define FC_CAN_HEAD_H

#include "mbed.h"

#include "FCState.h"

struct __attribute__((packed)) FCCANPacket 
{
    uint8_t _id; // id отправителя
    uint8_t _raw; // состояние обьекта оправителя
    char _data[6]; // произвольные данные отпраляемые конкретным обьектом

    FCCANPacket(uint8_t id = 0, uint8_t raw = 0, char *data = nullptr)
        : _id{},
          _raw{raw}
    {
        if(data)
        {
            memcpy(_data, data, 6);
        }    
    }
}; // = 8 байт

static_assert(sizeof(FCCANPacket) == 8, "FCCANPacket должен быть <= 8 байт !!!");

class FCCANBus 
    : public mbed::CAN, 
      public FCCondition  
{
private:
    FCCANPacket _packet;

public:
    FCCANBus(PinName rd, PinName td, uint32_t freq) 
        : mbed::CAN(rd, td) 
    {
        frequency(freq);
    }

    //! метод сканирование can шины 
    const string scan();

    int send(FCCANPacket packet);
    FCCANPacket receive();

    // обмен данными по CAN линии
    FCCANPacket& exchange(FCCANPacket &packet);
};

#endif // FC_CAN_HEAD_H
