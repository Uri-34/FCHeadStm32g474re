#ifndef I2C_STEP_DIR_ENGINE_H
#define I2C_STEP_DIR_ENGINE_H

#include <vector>

#include <DigitalOut.h>

#include "Head/Device/I2CDevice/I2CEngine/I2CEngine.h"

class I2CStepDirEngine : public I2CEngine 
{
public:
    enum Direction 
    {
        CounterClockwise,
        Clockwise
    };

private:
    // PinName _txPin; // tx порта
    // PinName _rxPin; // rx порта

    // PinName _stepPin; // пин управления шагами
    // PinName _directionPin; // пин управления направления вращения ротора 

    // uint32_t _baudRate; // скорость передачи данных в порт

    // DigitalOut _step; // пин для управления шагами
    Direction _direction; // направление вращения ротора 

public:
    I2CStepDirEngine(I2CBus *i2c, const string &name, uint8_t address) // , PinName tx, PinName rx, PinName step, PinName dir
        : I2CEngine(i2c, name, address) 
        //   _txPin{tx}, 
        //   _rxPin{rx}, 
        //   _stepPin{step}, 
        //   _directionPin{dir}, 
        //   _baudRate{baudRate}
    {}

    // inline uint32_t baudRate() 
    // { 
    //     return _baudRate; 
    // }

    void step() // сделать один шаг в направлении _direction  
    {
        // _step = 1;
        // wait_us(1); // Короткая задержка для формирования импульса
        // _step = 0;    
    }

    void steps(uint16_t stps) // сделать stps количество шагов в направлениеи _direction  
    {
        while(stps)
        {
            // ...
            
            stps--;
        }
    }

    inline Direction direction() // вернуть направление движения
    { 
        return _direction; 
    }

    inline void setDirection(const Direction dir) // установить направление движения  
    { 
        _direction = dir;
    } 

    // void (*stateCallBack)(const State &state) = 0;

    uint8_t crc(uint8_t* data, size_t length); // Расчет CRC
};

#endif // I2C_STEP_DIR_ENGINE_H