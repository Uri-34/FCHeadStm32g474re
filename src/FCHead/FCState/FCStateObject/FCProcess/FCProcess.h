#ifndef FC_PROCESS_H
#define FC_PROCESS_H

#include "FCStateObject.h"
#include "FCProcessID.h"

/*!
	\brief Класс для организации работы обьектов в отдельных потоках
*/
class FCProcess 
    : public FCStateObject
{
private:
    // приоритет процесса
    osPriority _priority;
    // задержка при выполнении цикла обработки процесса 
    Kernel::Clock::duration_u32 _delay;
    // таймер 
    Timer _timer;

public:
    //! const string &name - ссылка на имя обьекта
    FCProcess(const string &name, const FCProcessID id, osPriority priority, Kernel::Clock::duration_u32 delay) 
        : FCStateObject(name, id),
          _priority{priority},
          _delay{delay}
    {}

    //! метод проверяющий на совпадение идентификатор обьекта 
    inline bool match(FCProcessID id) { return FCStateObject::id() == id; }

    //! метод устанавливающий значение задержки
    void setDelay(Kernel::Clock::duration_u32 delay) 
    { 
        if(delay < 0ms)
        {
            _delay = 0ms;
        } 
        else
        {
            _delay = delay;
        }
    }

    //! метод возвращающий значение задержки
    inline Kernel::Clock::duration_u32 delay() { return _delay; } 

    //! метод приостановки работы на заданное время
    inline void sleep(Kernel::Clock::duration_u32 time) { ThisThread::sleep_for(time); } 

    //! метод возвращает приоритет процесса
    inline osPriority priority() { return _priority; }

    //! метод возвращает таймер процесса
    inline Timer& timer() { return _timer; } 

    //! метод выполнения последовательности действий при работе обьекта в отдельном потоке
    virtual void exec() = 0;
};
   
#endif // FC_PROCESS_H