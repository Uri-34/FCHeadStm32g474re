#ifndef STATE_H
#define STATE_H

#include <bitset>

#include "mbed.h"

//! перечисление возможных состояний
enum 
{
    Ready,      // 1 - готов к работе
                // 0 - не готов к работе

    Error,      // 1 - возникла ошибка
                // 0 - ошибки нет

    Started,    // 1 - выполнение установлена
                // 0 - выполнение сброшено   

    Paused,     // 1 - пауза установлена
                // 0 - пауза сброшена  

    Changed,    // 1 - внесены изменения
                // 0 - сброшенны изменения    
    
    FLAGS
};

typedef bitset<FLAGS> Condition; 

/*!
	\brief Класс состояния обьекта проекта
*/
class State 
    : public bitset<FLAGS>
{
private:
    Condition _condition;

public:
    State(Condition condition = Ready ) 
        : _condition{condition}
    {}

    //! метод возвращающий состояние 
    inline Condition condition() { return _condition; }

    //! метод проверяющий состояние
    inline bool is(size_t position) { return test(position); }
};

#endif // STATE_H