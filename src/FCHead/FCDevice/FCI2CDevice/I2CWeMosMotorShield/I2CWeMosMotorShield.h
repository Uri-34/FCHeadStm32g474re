#ifndef I2C_WE_MOS_2_MOTOR_DRIVER_H
#define I2C_WE_MOS_2_MOTOR_DRIVER_H

#include "Head/Device/I2CDevice/I2CDevice.h"

//! 0x10 - 0 двигатель
//! 0x11 - 1 двигатель  
enum WyMosMotorShieldMotorMode { First = 0x10, Second = 0x11 };

//! навравление вращения двигателя
enum WeMosMotorShieldMotorDirection { Clockwise = 0x02, CounterClockwise = 0x03 };

class TB6612FNGMotor
{
private:    
    union Data 
    {
        struct 
        {
            char _mode,
                 _direction,
                 _reserved,
                 _speed;
        } _target;
        char _array[4];
    } _data;

public:
    TB6612FNGMotor(uint8_t mode = 0x00, uint8_t direction = 0x00, uint8_t speed = 0x00);
    
    inline void clear() { *this = {0x00}; }
    
    void setFrequency(uint8_t frequency); 
    
    void setDirection(uint8_t motor, uint8_t direction); 
    
    void setSpeed(uint8_t motor, uint8_t speed); 
    
    inline const char* data() 
    { 
        return _data._array; 
    }
};

// адрес по умолчанию самого шилда
const uint8_t WE_MOS_MOTOR_SHIELD_DEFAULT_ADDRESS = 0x30;
// частота шилда    
const uint8_t WE_MOS_MOTOR_SHIELD_FREQUENCY = 0x08;
// минимальна скорость двигателя
const uint8_t WE_MOS_MOTOR_SHIELD_MIN_SPEED = 0;
// максимальная скорость двигателя
const uint8_t WE_MOS_MOTOR_SHIELD_MAX_SPEED = 64;
// стартовая скорость двигателя = половине максимальной
const uint8_t WE_MOS_MOTOR_SHIELD_START_SPEED = WE_MOS_MOTOR_SHIELD_MAX_SPEED/2;

class I2CWeMosMotorShield : public I2CDevice
{
private:    
    // массив моторов
    TB6612FNGMotor _motor[2];

public:
    I2CWeMosMotorShield(I2CBus *i2c, const string &name, uint8_t address = WE_MOS_MOTOR_SHIELD_DEFAULT_ADDRESS)
        : I2CDevice(i2c, name, address)
    {
        setFrequency(WE_MOS_MOTOR_SHIELD_FREQUENCY);
    }

    //! устанавливаем частоту работы моторов
    bool setFrequency(uint8_t frequency); 

    //! устанавливаем направление вращения мотора
    bool setDirection(uint8_t motor, uint8_t direction); 

    //! устанавливаем скорость работы мотора
    bool setSpeed(uint8_t motor, uint8_t speed); 

    //! запускаем мотор 
    void start(uint8_t motor);

    //! останавливаем мотор 
    void stop(uint8_t motor);
};

#endif // I2C_WE_MOS_2_MOTOR_DRIVER_H