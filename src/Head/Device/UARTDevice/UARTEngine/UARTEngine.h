#ifndef UART_ENGINE_H
#define UART_ENGINE_H

#include "Pins.h"
#include "Head/Device/UARTDevice/UARTDevice.h"

class UARTEngine : public UARTDevice 
{
public:
    UARTEngine(const string &name, const PinName &pin, uint8_t address) 
        : UARTDevice(name, pin, address)
    {}
};

#endif // UART_ENGINE_H