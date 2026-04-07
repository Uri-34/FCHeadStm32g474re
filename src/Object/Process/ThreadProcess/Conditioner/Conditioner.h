#ifndef CONDITIONER_H
#define CONDITIONER_H

#include "mbed.h"

#include "Pins.h"
#include "Common/Common.h"
#include "Object/Process/ThreadProcess/ThreadProcess.h"
#include "Head/Device/I2CDevice/I2CLM73/I2CLM73.h"
#include "Head/Device/I2CDevice/I2CFan/I2CFan.h"

/*!
	\brief Класс кондиционера для работы в отдельном потоке без использования очереди
*/

class ConditionerData
{
private:
    uint8_t _temperature; // температура
    uint8_t _speed; // обороты двигателя

public:
    ConditionerData(uint8_t temperature, uint8_t speed)
        : _temperature{temperature},
          _speed{speed}  
    {}

    inline uint8_t temperature() { return _temperature; }
    inline uint8_t speed() { return _speed; }
};

// class ConditionerMessage
//     : public Message
// {
// private:
//     ConditionerData _data;

// public:
//     ConditionerMessage(Process *source, ProcessID dest, Command command, ConditionerData data) 
//         : Message(source, dest, command),
//           _data{data}
//     {}

//     inline ConditionerData& data() { return _data; }
// };

class Conditioner 
    : public ThreadProcess
{
private:
    //! ошибка создания вентилятора 
    const string StartError = "Поток " + name() + "a не готов...";

    //! указатель на датчик температуры
    I2CLM73 *_lm73 = nullptr;

    //! указатель на вентилятор 
    I2CFan *_fan = nullptr;

public:
    Conditioner(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay);
   ~Conditioner(); 

    // метод обработки сообщений приходящих для данного обьекта
    // void handler(Message* message) override final;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // CONDITIONER_H