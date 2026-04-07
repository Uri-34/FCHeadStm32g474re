#ifndef PINS_H
#define PINS_H

#include "mbed.h"

// двигатели
// шаговые

// вентиляторы
const uint8_t MainFanI2CAddress = 0x48;; // рекомендуемый i2c адрес для драйвера управления вентирятором 
// const PinName FanPIN2        = PA_*; // 2 
// const PinName FanPIN3        = PA_*; // 3 
// const PinName FanPIN4        = PA_*; // 4 


// индикаторы
// WS2811F
const PinName MainIndicatorPIN  = PB_11;

// миксер
const PinName MixerPIN          = PA_0; // ???
const PinName MixerTX_PIN       = PA_9;
const PinName MixerRX_PIN       = PA_10;
const PinName MixerSTEP_PIN     = PB_6;
const PinName MixerDIR_PIN      = PB_7;

// пьезо-насосы
const PinName PiezoPumpPIN      = PA_0; // ???

// сенсоры
// I2C
const uint32_t LOW_I2C_SENSOR_FREQUENCY = 100000;
const uint32_t HIGH_I2C_SENSOR_FREQUENCY = 400000;

const uint8_t LM73I2CAddress    = 0x48; 

const PinName DistanceSensorPIN = PA_0; // ???

// SPI

#endif // PINS_H