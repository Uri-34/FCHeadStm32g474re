#ifndef SPI_BUS_H
#define SPI_BUS_H

#include "mbed.h"

class SPIBus : public mbed::SPI
{
private:

public:
    SPIBus(PinName mosi = SPI_MOSI, PinName miso = SPI_MISO, PinName sclk = SPI_CS) 
        : mbed::SPI(mosi, miso, sclk) 
    {}

    //! метод сканирование i2c шины 
    // const string detect(); // реализовать при необходимости 
};

#endif // SPI_BUS_H