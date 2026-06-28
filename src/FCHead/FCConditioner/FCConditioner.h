#ifndef FC_CONDITIONER_H
#define FC_CONDITIONER_H

#include "mbed.h"
#include "Pins.h"

#include "FCCommon.h"
#include "FCThreadedProcess.h"
#include "FCEMC2101.h"
// #include "FCI2CLM75.h"
// #include "FCI2CFan.h"

// /*!
// 	\brief Класс кондиционера для работы в отдельном потоке
// */
class FCConditioner 
    : public FCThreadedProcess
{
private:
    struct FCConditionerData
    {
        string _name;
        uint8_t _temperature,
                _speed;    
    
        FCConditionerData(string name, uint8_t temperature, uint8_t speed)
            : _name{name},
              _temperature{temperature},
              _speed{speed}
        {}            
    };

    // const uint8_t TermometerAddress = 0x48; 
    // const uint8_t FanAddress = 0x49; 

    // //! ошибка создания вентилятора 
    // const string StartError = "Поток " + name() + "a не готов...";

    //! указатель на датчик температуры
    // FCI2CLM75 *_termometer = nullptr;

    //! указатель на вентилятор 
    // FCI2CFan *_fan = nullptr;

    //! указатель на контроллер вентилятор+датчик температуры 
    FCEMC2101 *_emc2101 = nullptr;

public:
    FCConditioner(FCI2CBus &i2c, uint8_t address, string name, FCHeadQueue &queue, Kernel::Clock::duration_u32 delay = 100ms); 

   ~FCConditioner(); 

    // метод обработки сообщений приходящих для данного обьекта
    // void handler(Message* message) override final;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // FC_CONDITIONER_H
