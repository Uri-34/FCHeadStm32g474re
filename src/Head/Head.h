#ifndef HEAD_H
#define HEAD_H

#include "I2CBus.h"
//#include "SPIBus.h"
//#include "UARTBus.h"
#include "CANBus.h"

#include "Process.h"
#include "Message.h"
#include "ThreadProcess.h"

//! класс для организации совместной работы всех компонентов головки 
class Head 
    : public Process
{
private:
    const string NotReadyString = "не готов";
    const string NotStartedString = "не запущен";
    const string EllipsisString = "...";
    const string DoubleComaString = ":";
    
    //! i2c шина
    //          I2C_SDA I2C_SCL 
    I2CBus _i2c{PB_9,   PB_8};
    const uint32_t I2C_FREQUENCY = 100000;
    
    //! spi шина 
    //          SPI_MOSI  SPI_MISO  SPI_CS    
//   SPIBus _spi{PA_7,     NC,       PA_5}; //    V
//   const uint32_t SPI_FREQUENCY = 40000000; //(40 MHz, 60 MHz, 80 MHz)

    //! uart шина
    //            USBTX USBRX    
//    UARTBus _uart{PA_2, PA_3};
//    const int UART_BAUDRATE = 115200;

    //! can шина 
    CANBus _can{PA_11, PA_12};
    const uint32_t CAN_FREQUENCY = 500000;

    //! список запускаемых в отдельных потоках обьектов 
    ThreadProcesses _processes;

    // очередь для обмена сообщениями иежду потоками
    HeadQueue _queue;

 public:
    Head();

    //! метод главного цикла
    void exec() override;
};

#endif // HEAD_H