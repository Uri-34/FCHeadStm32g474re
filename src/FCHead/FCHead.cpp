#include "mbed.h"

#include "FCHead.h"

#include "FCProcess.h"
#include "FCTerminal.h"

#include "FCConditioner.h"

#include "FCCANBus.h"

#include "FCFeeder.h"

FCHead::FCHead() 
    : FCProcess("Головка", HeadID, osPriorityNormal, 1000ms)
{
    init();
}

void FCHead::init()
{
    // фидеры
    _feeders = new FCFeeders(_i2c);

    // кондиционеры
    _conditioners << new FCConditioner(_i2c, 0x70, "c0", _queue);
    _conditioners << new FCConditioner(_i2c, 0x71, "c1", _queue);
    _conditioners << new FCConditioner(_i2c, 0x72, "c2", _queue);

    // проверка состояний шин, кондиционеров, фидеров и установка состояния головки в целом
    set(_i2c.is(FCState::Ready) && _can.is(FCState::Ready) && 
        _conditioners.is(FCState::Ready) && _feeders->is(FCState::Ready) ? FCState::Ready : FCState::NotReady);

    // может быть так и оставлю в качестве heart-beet !!!     
    mbed_event_queue()->call_every(std::chrono::seconds(2), [this]() 
                                                            { 
                                                                Msg("Heartbeet раз в 2 сек...");
                                                                _can.send({id(), getRaw(), {}}); 
                                                            });
}

#include "platform/mbed_stats.h"
void FCHead::exec()
{
    // если нет общей готовности отправляем пакет в CAN
    // _can.send({id(), getRaw(), {}});

    // для отслеживания утечки памяти (ОТЛАДКА)
    // mbed_stats_heap_t info;

    Msg("Стартовала: %s", name().c_str());

    FCRange<uint32_t> queueLimit(3, _queue.size() - 3); 

    int step = 0;
    while(true) 
    {
        // читаем очередь

        // если сообщение необходимо транслировать rpi то отправляем его в can шину
        
        // если сообщение отправленно для Head, то обрабатываем его 
        
        // проверяем на истечение времени таймера и отправляем heartbeet сообщение rpi

        if(is(FCState::Ready))
        {
            Msg("%s готова: шаг - %d", name().c_str(), step);
            step++;
        }
        else    
        {
            Msg("%s не готова !!!", name().c_str());
        }

        // printInfo(info);

        // if(!queueLimit.inrange(messages))
        // {
        //     _processes.balancer(&_queue);
        // }

        sleep(delay());
    }
}

void FCHead::printInfo(mbed_stats_heap_t &info)
{
    mbed_stats_heap_get(&info);
    Msg("Текущее значение кучи: %lu байт", info.current_size);
    Msg("Используемый размер кучи: %lu байт", info.max_size);
    Msg("Зарезервированный размер кучи: %lu байт", info.reserved_size);
    Msg("Счетчик распределенных блоков: %lu", info.alloc_cnt);
    Msg("Фрагментация: %lu байт", info.overhead_size);

    static uint32_t value = 0;
    Msg("Индикация работы потока %s раз в %d сек: v = %d", name().c_str(), delay()/1000, value++);

    auto messages = _queue.count();
    Msg("В очереди - %d сообщений", messages);
}