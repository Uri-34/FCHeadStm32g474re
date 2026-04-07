#ifndef BME280v33_SENSOR_H
#define BME280v33_SENSOR_H

#include "Head/Device/I2CDevice/I2CDevice.h"

struct BME280Data
{
    int32_t _temperature;
    int32_t _pressure;
    int32_t _humidity;

    BME280Data(int32_t temperature = 0.0, int32_t pressure = 0.0, int32_t humidity = 0.0)
        : _temperature{temperature},
          _pressure{pressure},
          _humidity{humidity}  
    {}
};

class BME280v33 : public I2CDevice
{
private:
    BME280Data _data;

    // ==== Регистры ====
    const char BME280v33_WHO_AM_I      = 0x60;

    const char BME280v33_REG_ID        = 0xD0;
    const char BME280v33_REG_RESET     = 0xE0;
    const char BME280v33_REG_CTRL_HUM  = 0xF2;
    const char BME280v33_REG_CTRL_MEAS = 0xF4;
    const char BME280v33_REG_CONFIG    = 0xF5;
    const char BME280v33_REG_PRESS_MSB = 0xF7;
    const char BME280v33_REG_TEMP_MSB  = 0xFA;
    const char BME280v33_REG_HUM_MSB   = 0xFD;

    // ==== Калибровочные регистры ====
    const char BME280v33_REG_DIG_T1    = 0x88;
    const char BME280v33_REG_DIG_T2    = 0x8A;
    const char BME280v33_REG_DIG_T3    = 0x8C;

    const char BME280v33_REG_DIG_P1    = 0x8E;
    const char BME280v33_REG_DIG_P2    = 0x90;
    const char BME280v33_REG_DIG_P3    = 0x92;
    const char BME280v33_REG_DIG_P4    = 0x94;
    const char BME280v33_REG_DIG_P5    = 0x96;
    const char BME280v33_REG_DIG_P6    = 0x98;
    const char BME280v33_REG_DIG_P7    = 0x9A;
    const char BME280v33_REG_DIG_P8    = 0x9C;
    const char BME280v33_REG_DIG_P9    = 0x9E;

    const char BME280v33_REG_DIG_H1    = 0xA1;
    const char BME280v33_REG_DIG_H2    = 0xE1;
    const char BME280v33_REG_DIG_H3    = 0xE3;
    const char BME280v33_REG_DIG_H4    = 0xE4;
    const char BME280v33_REG_DIG_H5    = 0xE5;
    const char BME280v33_REG_DIG_H6    = 0xE7;

    // ==== Калибровочные коэффициенты ====
    uint16_t dig_T1;
    int16_t dig_T2, dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

    uint8_t dig_H1;
    int16_t dig_H2, dig_H3, dig_H4, dig_H5;
    int8_t dig_H6;

    int32_t t_fine = 0; // используется в компенсации

    // -------------------------------------

    const string ErrorExist = "датчик не доступен...";

    void calibration();
    float compensateTemperature(int32_t adc_T);
    float compensatePressure(int64_t adc_P);
    float compensateHumidity(int32_t adc_H);

public:
    BME280v33(I2CBus *i2c, const string &name, const uint8_t address);

    const BME280Data& read();
};

#endif // BME280v33_SENSOR_H