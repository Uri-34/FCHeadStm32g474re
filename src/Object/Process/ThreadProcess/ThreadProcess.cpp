#include "Object/Process/ThreadProcess/ThreadProcess.h"
#include "Common/Common.h"

bool ThreadProcess::sendMessage(ProcessID dest, Command command, void *data)
{
    Message *message = new Message(this, GMT147SPIID, Show, data);
    if(!queue()->send(message))
    {
        Err("сообщение от " + name() + " не добавленно в очередь !!!");
        delete message;
        return false;
    }    
    return true;
}    

void ThreadProcess::exec()
{
    timer().start();

    while(true)
    {
        if(!is(Started))
        {
            Msg("Поток " + name() + " не запущен... ");
            continue;            
        }    

        // проверяем на наличие и соответствие сообщения данному обьекту
        // обрабатываем переданные данные
        Message* message = queue()->receive();    
        if(message) 
        {
            if(iAm(message))
            {
                handle(message);
            }
            else
            {
                // Msg("возвращаем сообщение обьекта %s в очередь", message->source()->name().c_str());
                if(!queue()->send(message))
                {
                    Err("ОЧЕРЕДЬ ПЕРЕПОЛНЕНА !!!");
                }
            }
        }
        
        // проверяем таймер на истечении delay() и выполняем код generate() класса наследника 
        if(timer().elapsed_time() >= delay())
        {
            timer().reset();

            generate();
        }
    }
}

ThreadProcesses::~ThreadProcesses()
{
    for(auto &process : *this)
    {
        delete process;
    }
} 

bool ThreadProcesses::validate(ProcessID id) const
{ 
    for (const auto& process : *this) 
    {
        if (process && process->match(id)) 
        {  
            return true;
        }
    }    
    return false;
}

bool ThreadProcesses::start()
{
    for(int count = 0; count < (int)(*this).size(); count++)
    {
        ThreadProcess* process = (*this)[count];
        if(!process)
        {
            return false;
        }

        Thread *thread = new Thread(process->priority());
        if(!thread)
        {
            return false;
        }

        osStatus status = thread->start([process](){ process->exec(); });
        if(status != osOK)
        {
            return false;
        }
        
        Msg("%d: запущен процесс - %s", count, process->name().c_str());
    }    
return true;
}

bool ThreadProcesses::balancer(HeadQueue *queue)
{
    // bool result = false;
    // for(int count = 1; count < (int)(*this).size(); count++)
    // {
    //     ThreadProcess* process = (*this)[count];
    //     if(process->id() != GMT147SPIID)
    //     {
    //         continue;
    //     }    

    //     uint8_t fill = queue->filled();
    //     Kernel::Clock::duration_u32 delay = process->delay();
    //     if(fill > 80)
    //     {
    //         // увеличиваем задержку на StepSizePercent процентов
    //         process->setDelay(delay + ((delay / 100) * StepSizePercent));
    //         result = true;
    //     }

    //     if(fill < 20)
    //     {
    //         // уменьшаем задержку на StepSizePercent процентов
    //         process->setDelay(delay - ((delay / 100) * StepSizePercent));
    //         result = true;
    //     }
    //     Msg("_delay() = %lu", delay);
    // }        
    return true;
}
