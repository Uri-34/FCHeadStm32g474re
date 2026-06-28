#ifndef FC_CONDITIONER_H
#define FC_CONDITIONER_H

#include "mbed.h"
#include "Pins.h"

#include "FCCommon.h"
#include "FCThreadProcess.h"
#include "FCI2CLM75.h"
#include "FCI2CFan.h"

// /*!
// 	\brief Класс кондиционера для работы в отдельном потоке без использования очереди
// */
class FCConditioner 
    : public FCThreadProcess
{
private:
    const uint8_t LM75I2CAddress = 0x48; 

    // //! ошибка создания вентилятора 
    // const string StartError = "Поток " + name() + "a не готов...";

    //! указатель на датчик температуры
    FCI2CLM75 *_termometr = nullptr;

    //! указатель на вентилятор 
    FCI2CFan *_fan = nullptr;

public:
    FCConditioner(FCI2CBus *i2c, uint8_t LM75Address, FCHeadQueue *queue, Kernel::Clock::duration_u32 delay);
   ~FCConditioner(); 

    // метод обработки сообщений приходящих для данного обьекта
    // void handler(Message* message) override final;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // FC_CONDITIONER_H
