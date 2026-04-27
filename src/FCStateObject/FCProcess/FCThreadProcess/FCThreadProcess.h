#ifndef FC_THREAD_PROCESS_H
#define FC_THREAD_PROCESS_H

#include "FCProcess.h"
#include "FCProcessID.h"
#include "FCHeadQueue.h"

/*!
	\brief Класс для организации работы обьектов в отдельных потоках с опросом очереди
*/
class FCThreadProcess 
    : public FCProcess
{
private:
    //! указатель на очередь для обмена сообщениями
    FCHeadQueue *_queue = nullptr; 
    
public:
    FCThreadProcess(FCHeadQueue *queue, const string &name, const FCProcessID id, 
                 osPriority priority, Kernel::Clock::duration_u32 delay) 
        : FCProcess(name, id, priority, delay),
          _queue{queue}
    {
        set(FCReadyState::Ready);
    }

    //! метод проверки на свой/чужой 
    inline bool iAm(FCMessage* message) { return id() == message->destId(); }

    //! метод возвращающий указатель на очередь
    inline FCHeadQueue* queue() { return _queue; }

    //! отправка сообщения обьекта-потока другому обьекту-потоку
    bool sendMessage(FCProcessID dest, FCCommand command, void *data);

    //! метод выполнения последовательности действий при работе обьекта в отдельном потоке
    virtual void exec() override;

    //! виртуальный метод выполнения последовательности действий характерных 
    //! для данного обьекта при работе обьекта в отдельном потоке с очереью
    virtual void handle(FCMessage* message) {}

    //! виртуальный метод выполнения последовательности действий по таймеру 
    virtual void generate() {}
};

//! класс для хранения списка запускаемых в отдельных потоках обьектов для работы с очередью
class FCThreadProcesses 
    : public vector<FCThreadProcess*>
{
private:
public:
    FCThreadProcesses() 
        : vector<FCThreadProcess*>()
    {}

   ~FCThreadProcesses();

    //! метод для добавления обьектов в список
    inline void operator << (FCThreadProcess* process) { push_back(process); }
    //! метод проверки наличия обьекта в списке 
    bool validate(FCProcessID id) const; 
    //! инициализация потоков и старт всех процессов помещенных в список процессов
    bool start();
    //! балансирует нагрузку на изменяя задержку delay() в процессах
    bool balancer(FCHeadQueue *queue);
};

#endif // FC_THREAD_PROCESS_H