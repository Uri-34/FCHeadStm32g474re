#ifndef I2C_FAN_H
#define I2C_FAN_H

#include "Common/Common.h"
#include "Head/Device/I2CDevice/I2CDevice.h"

/*!
	\brief Класс для организации работы обьекта вентилятора работающего по I2C интерфейсу
*/
class I2CFan : public I2CDevice
{
private:
    //! константа - минимальная скорость
    const float MIN_SPEED = 0.0f;

    //! константа - максимальная скорость
    const float MAX_SPEED = 1.0f;

    //! скорость вращения вентилятора  
    float _speed = 0.0;

public:
    //! константа - ошибка готовности вентилятора 
    const string ErrorFan = "вентилятор не готов...";

    I2CFan(I2CBus *i2c, const string &name, uint8_t address)  
        : I2CDevice{i2c, name, address}
    {}   

    virtual ~I2CFan()  
    {}   

    //! метод установки скорости вращения вентилятора от 0 до 1 
    void setSpeed(float speed) 
    { 
        _speed = bound(MIN_SPEED, speed, MAX_SPEED); 
        
        // write(_speed); --- сделать нормальный вызов write()
    }

    //! метод установки скорости вращения вентилятора от 0% до 100% 
    inline void setSpeedPercent(int percent) 
    { 
        setSpeed(percent/100);
    }
        
    //! метод возвращающий скорость вращения вентилятора от 0 до 1 
    inline float speed() { return _speed; }

    bool ready() override final { return true; };
};

#endif // I2C_FAN_H