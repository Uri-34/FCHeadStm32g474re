#ifndef UART_BUS_H
#define UART_BUS_H

#include "mbed.h"

class UARTBus : public mbed::BufferedSerial
{
private:

public:
    UARTBus(PinName tx, PinName rx) : mbed::BufferedSerial(tx, rx) {}

    //! метод сканирование uart шины 
    const string detect(); 
};

#endif // UART_BUS_H