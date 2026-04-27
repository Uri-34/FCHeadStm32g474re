#ifndef THREAD_PROCESS_H
#define THREAD_PROCESS_H

#include "Process.h"
#include "ProcessID.h"
#include "HeadQueue.h"

/*!
	\brief Класс для организации работы обьектов в отдельных потоках с опросом очереди
*/
class ThreadProcess 
    : public Process
{
private:
    //! указатель на очередь для обмена сообщениями
    HeadQueue *_queue = nullptr; 
    
public:
    ThreadProcess(HeadQueue *queue, const string &name, const ProcessID id, 
                 osPriority priority, Kernel::Clock::duration_u32 delay) 
        : Process(name, id, priority, delay),
          _queue{queue}
    {
        set(Ready);
    }

    //! метод проверки на свой/чужой 
    inline bool iAm(Message* message) { return id() == message->destId(); }

    //! метод возвращающий указатель на очередь
    inline HeadQueue* queue() { return _queue; }

    //! отправка сообщения обьекта-потока другому обьекту-потоку
    bool sendMessage(ProcessID dest, Command command, void *data);

    //! метод выполнения последовательности действий при работе обьекта в отдельном потоке
    virtual void exec() override;

    //! виртуальный метод выполнения последовательности действий характерных 
    //! для данного обьекта при работе обьекта в отдельном потоке с очереью
    virtual void handle(Message* message) {}

    //! виртуальный метод выполнения последовательности действий по таймеру 
    virtual void generate() {}
};

//! класс для хранения списка запускаемых в отдельных потоках обьектов для работы с очередью
class ThreadProcesses 
    : public vector<ThreadProcess*>
{
private:
public:
    ThreadProcesses() 
        : vector<ThreadProcess*>()
    {}

   ~ThreadProcesses();

    //! метод для добавления обьектов в список
    inline void operator << (ThreadProcess* process) { push_back(process); }
    //! метод проверки наличия обьекта в списке 
    bool validate(ProcessID id) const; 
    //! инициализация потоков и старт всех процессов помещенных в список процессов
    bool start();
    //! балансирует нагрузку на изменяя задержку delay() в процессах
    bool balancer(HeadQueue *queue);
};

#endif // THREAD_PROCESS_H