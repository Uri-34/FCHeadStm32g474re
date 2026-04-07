#include "Head/Device/I2CDevice/I2CWeMosMotorShield/I2CWeMosMotorShield.h"

TB6612FNGMotor::TB6612FNGMotor(uint8_t mode, uint8_t direction, uint8_t speed)
    {
        _data._target._mode = contains<uint8_t>(mode, vector<uint8_t>{First, Second});       
        _data._target._direction = contains<uint8_t>(direction, vector<uint8_t>{Clockwise, CounterClockwise});
        _data._target._reserved = 0x00;
        _data._target._speed = bound<uint8_t>(WE_MOS_MOTOR_SHIELD_MIN_SPEED, speed, WE_MOS_MOTOR_SHIELD_MAX_SPEED);
    }

void TB6612FNGMotor::setFrequency(uint8_t frequency) 
{ 
    clear(); 
    _data._target._direction = frequency;

}

void TB6612FNGMotor::setDirection(uint8_t motor, uint8_t direction) 
{ 
    clear(); 
    _data._target._mode = contains<uint8_t>(motor, vector<uint8_t>{First, Second}); 
    _data._target._direction = contains<uint8_t>(direction, vector<uint8_t>{Clockwise, CounterClockwise}); 
}

void TB6612FNGMotor::setSpeed(uint8_t motor, uint8_t speed) 
{ 
    clear(); 
    _data._target._mode = contains<uint8_t>(motor, vector<uint8_t>{First, Second}); 
    _data._target._speed = bound<uint8_t>(WE_MOS_MOTOR_SHIELD_MIN_SPEED, speed, WE_MOS_MOTOR_SHIELD_MAX_SPEED); 
}

// -----------------------------------------------------------

bool I2CWeMosMotorShield::setFrequency(uint8_t frequency) 
{ 
    TB6612FNGMotor fr;
    fr.setFrequency(frequency);
    return write(fr.data() , 4);
}    

bool I2CWeMosMotorShield::setDirection(uint8_t motor, uint8_t direction) 
{ 
    uint8_t motorNumber = bound<uint8_t>(0, motor, 1);

    _motor[motorNumber].setDirection(motorNumber, direction);
    return write(_motor[motorNumber].data(), 4);
}

bool I2CWeMosMotorShield::setSpeed(uint8_t motor, uint8_t speed) 
{ 
    uint8_t m = bound<uint8_t>(0, motor, 1);
    uint8_t s = bound<uint8_t>(WE_MOS_MOTOR_SHIELD_MIN_SPEED, speed, WE_MOS_MOTOR_SHIELD_MAX_SPEED);

    _motor[m].setSpeed(m, s);
    return write(_motor[m].data(), 4);
}

void I2CWeMosMotorShield::start(uint8_t motor)
{
    write(_motor[bound<uint8_t>(0, motor, 1)].data(), 4); // протестировать и дописать процесс старта
}

void I2CWeMosMotorShield::stop(uint8_t motor)
{
    write(_motor[bound<uint8_t>(0, motor, 1)].data(), 4); // протестировать и дописать остановку
}
