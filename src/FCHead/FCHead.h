#ifndef FC_HEAD_H
#define FC_HEAD_H

#include "FCI2CBus.h"
#include "FCCANBus.h"

#include "FCProcess.h"
#include "FCMessage.h"
#include "FCThreadProcess.h"

//! класс для организации совместной работы всех компонентов головки 
class FCHead 
    : public FCProcess
{
 public:
    FCHead();

    //! метод главного цикла
    void exec() override;

private:
    const string NotReadyString = "не готов";
    const string NotStartedString = "не запущен";
    const string EllipsisString = "...";
    const string DoubleComaString = ":";
    
    //! i2c шина
    //          I2C_SDA I2C_SCL 
    FCI2CBus _i2c{PB_9,   PB_8};
    const uint32_t I2C_FREQUENCY = 100000;
    
    //! can шина 
    FCCANBus _can{PA_11, PA_12};
    const uint32_t CAN_FREQUENCY = 500000;

    //! список запускаемых в отдельных потоках обьектов 
    FCThreadProcesses _processes;

    // очередь для обмена сообщениями иежду потоками
    FCHeadQueue _queue;
};

#endif // FC_HEAD_H