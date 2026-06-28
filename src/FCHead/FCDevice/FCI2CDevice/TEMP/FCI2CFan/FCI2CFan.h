#ifndef FC_I2C_FAN_H
#define FC_I2C_FAN_H

#include "FCCommon.h"
#include "FCI2CDevice.h"

/*!
	\brief Класс для организации работы обьекта вентилятора работающего по I2C интерфейсу
*/
class FCI2CFan 
    : public FCI2CDevice
{
private:
    //! рекомендуемый i2c адрес для драйвера управления вентирятором
    const uint8_t MainFanI2CAddress = 0x48;;  

    //! константа - минимальная скорость
    const float MIN_SPEED = 0.0f;

    //! константа - максимальная скорость
    const float MAX_SPEED = 1.0f;

    //! скорость вращения вентилятора  
    float _speed = 0.0;

public:
    //! константа - ошибка готовности вентилятора 
    const string ErrorFan = "вентилятор не готов...";

    FCI2CFan(FCI2CBus *i2c, const string &name, uint8_t address)  
        : FCI2CDevice{i2c, name, address}
    {}   

    //! метод установки скорости вращения вентилятора от 0 до 1 
    void setSpeed(float speed) 
    { 
        _speed = bound(MIN_SPEED, speed, MAX_SPEED); 
        
        // write(_currSpeed); --- сделать нормальный вызов write()
    }

    //! метод возвращающий скорость вращения вентилятора от 0 до 1 
    inline float speed() { return _speed; }

    // сервисные методы для удобства
    // включение на предыдущей скорости
    inline void on() 
    { 
        // write(_currSpeed); --- сделать нормальный вызов write() 
    }
    // выключение
    inline void off() 
    { 
        // write(0.0); --- сделать нормальный вызов write()
    }
};

#endif // FC_I2C_FAN_H