#include "Head/Device/I2CDevice/BME280v33/BME280v33.h"

BME280v33::BME280v33(I2CBus *i2c, const string &name, uint8_t address) 
    : I2CDevice(i2c, name, address)
{}

// bool BME280v33::init() 
// { 
//     if(!exist())
//     {
//         return false;
//     }

//     // Установка режимов
//     i2c()->write(address()<<1, &BME280v33_REG_CTRL_HUM, 0x01);
//     i2c()->write(address()<<1, &BME280v33_REG_CTRL_MEAS, 0x27);
//     i2c()->write(address()<<1, &BME280v33_REG_CONFIG, 0xA0);

//     calibration();

//     return true; 
// }

void BME280v33::calibration() 
{
    char data[2];

    // Температурные коэффициенты
    i2c()->write(address()<<1, &BME280v33_REG_DIG_T1, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_T1 = (data[1] << 8) | data[0];

    i2c()->write(address()<<1, &BME280v33_REG_DIG_T2, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_T2 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_T3, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_T3 = (int16_t)((data[1] << 8) | data[0]);

    // Барометрические коэффициенты
    i2c()->write(address()<<1, &BME280v33_REG_DIG_P1, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P1 = (uint16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P2, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P2 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P3, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P3 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P4, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P4 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P5, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P5 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P6, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P6 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P7, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P7 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P8, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P8 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_P9, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_P9 = (int16_t)((data[1] << 8) | data[0]);

    // Влажностные коэффициенты
    i2c()->write(address()<<1, &BME280v33_REG_DIG_H1, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H1 = data[0];

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H2, 1, true);
    i2c()->read(address()<<1, data, 2);
    dig_H2 = (int16_t)((data[1] << 8) | data[0]);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H3, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H3 = data[0];

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H4, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H4 = (int16_t)(data[0] << 4);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H4 + 1, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H4 |= (data[0] & 0x0F);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H5, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H5 = (int16_t)((data[0] & 0xF0) << 4);

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H5 + 1, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H5 |= data[0];

    i2c()->write(address()<<1, &BME280v33_REG_DIG_H6, 1, true);
    i2c()->read(address()<<1, data, 1);
    dig_H6 = (int8_t)data[0];
}

float BME280v33::compensateTemperature(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) * (int32_t)dig_T3) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return (float)T / 100.0f;
}

float BME280v33::compensatePressure(int64_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + ((int64_t)dig_P4 << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

    if (var1 == 0)
        return 0;

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + ((int64_t)dig_P7 << 4);
    return (float)p / 256.0f;
}

float BME280v33::compensateHumidity(int32_t adc_H)
{
    int32_t h = t_fine - 76800;
    h = ((((adc_H << 14) - (((int32_t)dig_H5) << 20) - dig_H6 * h) +
           ((int32_t)1 << 15)) * dig_H2) >> 12 *    //) * 
          ((((h * dig_H3) >> 9) + ((int32_t)1 << 15)) >> 16);
    h = h * (1 - (dig_H1 * h >> 32));
    h = h >> 10;
    return (float)h / 1024.0f;
}

const BME280Data& BME280v33::read()
{
    if(condition() == Started)
    {
        char msbRegister = BME280v33_REG_TEMP_MSB;
        char data[8];

        i2c()->write(address()<<1, &msbRegister, 1, true);
        i2c()->read(address()<<1, data, 8);    

        int32_t adc_T = (int32_t)(((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2]) >> 4;
        int32_t adc_P = (int32_t)(((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5]) >> 4;
        int32_t adc_H = (int32_t)((uint16_t)data[6] << 8 | data[7]);

        _data._temperature = compensateTemperature(adc_T);
        _data._pressure = compensatePressure(adc_P);
        _data._humidity = compensateHumidity(adc_H);
    }

    return _data;
}
