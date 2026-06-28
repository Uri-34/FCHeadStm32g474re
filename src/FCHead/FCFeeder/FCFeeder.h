// #include "mbed.h"
// #include "FCA4988StepDirMotor.h"
// #include "FCFeedersManager.h"

// int main() 
// {
//     FCFeedersManager manager("FeedersManager");
//     manager << FCA4988StepDirMotor(MotorPins(PA_5, PA_6, PA_7),  "f0"));
//     manager << FCA4988StepDirMotor(MotorPins(PA_8, PA_9, PA_10), "f1"));

//     // инициализация только после старта RTOS
//     manager.init(); 

//     // Переключение фидеров
//     manager.switchTo(0);
//     ThisThread::sleep_for(2s);
//     manager.switchTo(1);
    
//     while(true) 
//     { 
//          ThisThread::sleep_for(1s);
//     }     
// }

#ifndef FC_FEEDER_H
#define FC_FEEDER_H

#include "FCA4988StepDirMotorWithEndStop.h"

class FCFeeder 
    : public FCA4988StepDirMotorWithEndStop
{
public:
    FCFeeder(FCI2CBus &bus, FCStateObject *parent = nullptr)
        : FCA4988StepDirMotorWithEndStop(&bus, 0x20, 0x21, 0, PA_0, PA_1, {}, parent)
    {}
};

#include <vector>
#include <string>

#include "mbed.h"
#include "FCProcess.h"
#include "FCDriver.h"
#include "FCCommon.h"
#include "FCFeeder.h"

class FCFeeders 
    : public FCProcess 
{
public:
    FCFeeders(FCI2CBus &i2c, string name = {}, FCStateObject *parent = nullptr);
    ~FCFeeders();

    //! переключение на фидер с номером feeder (0-based index)
    //! возвращает true при успешном завершении перемещения
    bool switchTo(uint8_t feeder);

    //! возвращает номер текущего активного фидера (UINT8_MAX если нет)
    uint8_t numberActiveFeeder() const;

    //! возвращает указатель на текущий активный фидер
    FCFeeder* ptrToActiveFeeder() const;

    //! инициализация: поиск Home для всех фидеров (вызывать ПОСЛЕ запуска RTOS)
    bool init();

protected:
    // если FCProcess требует реализации потока:
    void exec() override;

private:
    bool waitForMoveComplete(FCFeeder* f, uint32_t timeout_ms);
    bool moveToHome(FCFeeder* f);
    bool moveToWork(FCFeeder* f);

    mutable rtos::Mutex _mutex;
    std::vector<FCFeeder*> _feedersVector;
    uint8_t _currentFeeder;
    uint8_t _activeFeeder; // UINT8_MAX = нет активного

    // настройки механики
    static constexpr uint16_t WORK_POSITION_STEPS = 120; // Шагов от Home в рабочую зону
    static constexpr uint32_t MOVE_TIMEOUT_MS     = 6000;
};

#endif // FC_FEEDER_H