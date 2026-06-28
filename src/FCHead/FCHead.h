#ifndef FC_HEAD_H
#define FC_HEAD_H

#include <list>

#include "FCI2CBus.h"
#include "FCCANBus.h"

#include "FCProcess.h"
#include "FCMessage.h"
#include "FCThreadedProcess.h"

#include "FCFeeder.h"

#include "FCSignalConnection.h"

// #include "FCZAxis.h"

//! класс для организации совместной работы всех компонентов головки 
class FCHead 
    : public FCProcess
{
public:
    FCHead();

    //! метод главного цикла
    void exec() override;

private:
    void init();
    void printInfo(mbed_stats_heap_t &info);
    
    // инициализация шин 
    //! i2c шина  I2C_SDA  I2C_SCL  I2C_FREQUENCY
    FCI2CBus _i2c{PB_9,    PB_8,    100000};

    //! can шина  RX       TX
    FCCANBus _can{PA_11,   PA_12,   500000};

    // очередь для обмена сообщениями иежду потоками
    FCHeadQueue _queue;

    //! ось Z
    // FCZAxis *_zAxis = nullptr;

    //! менеджер фидеров
    FCFeeders *_feeders = nullptr;

    //! список запускаемых в отдельных потоках обьектов 
    FCThreadedProcesses _conditioners;
};

#endif // FC_HEAD_H