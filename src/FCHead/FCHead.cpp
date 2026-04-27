#include "Head.h"

#include "mbed.h"

#include "Process.h"

#include "Conditioner.h"
//#include "Object/Process/ThreadProcess/SPIDisplay/GMT147SPI/GMT147SPI.h"
#include "LaserScaner.h"
#include "LedIndicator.h"
#include "Mixer.h"
#include "Common.h"

Head::Head() 
    : Process("Головка:", HeadID, osPriorityNormal, 1000ms)
{
    // устанавливает частоты и скорости
    // устанавливаем частоту I2C интерфейса...
    _i2c.frequency(I2C_FREQUENCY);

    // устанавливаем частоту SPI интерфейса...
//    _spi.frequency(SPI_FREQUENCY); // 40 МГц
    
    // устанавливаем частоту UART интерфейса...
//    _uart.set_baud(UART_BAUDRATE);

    // устанавливаем частоту CAN интерфейса...
    _can.frequency(CAN_FREQUENCY);

    // инициализируем и добавляем процессы
    
    // работают без опроса очереди
    // ...
    
    // работают с опросом очереди
//    _processes << new GMT147SPI(&_spi, &_queue, 0ms);

    _processes << new Mixer(&_i2c, &_queue, 100ms);
    _processes << new Conditioner(&_i2c, &_queue, 1000ms);
    _processes << new LaserScanner(&_i2c, &_queue, 100ms);
    _processes << new LedIndicator(&_queue, 1000ms);
}

#include "platform/mbed_stats.h"
void Head::exec()
{
    mbed_stats_heap_t info;

    _processes.start() ? set(Started) : reset(Started);

    Msg("процессы стартовали...");

    Range<uint32_t> queueLimit(3, _queue.size() - 3); 

    while(true) 
    {
        if(!is(Started))
        {
            Msg("Поток " + name() + "не запущен... ");
            continue;            
        }    

        mbed_stats_heap_get(&info);

        Msg("Текущее значение кучи: %lu байт", info.current_size);
        // Msg("Используемый размер кучи: %lu байт", info.max_size);
        // Msg("Зарезервированный размер кучи: %lu байт", info.reserved_size);
        // Msg("Счетчик распределенных блоков: %lu", info.alloc_cnt);
        // Msg("Фрагментация: %lu байт", info.overhead_size);

        // static uint32_t value = 0;
        // Msg("Индикация работы потока " + name() + "раз в %d сек: v = %d", delay()/1000, value++);

        uint32_t messages = _queue.count();
        Msg("В очереди - %d сообщений", messages);

        if(!queueLimit.inrange(messages))
        {
            _processes.balancer(&_queue);
        }

        sleep(delay());
    }
}