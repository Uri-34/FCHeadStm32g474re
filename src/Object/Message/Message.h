#ifndef MESSAGE_H
#define MESSAGE_H

#include "Process.h"

// // стандартные сообщения об ошибке
// const string QueueMessageErrorUpdate = "ошибка добавления в очередь...";
// const string QueueMessageErrorGet = "ошибка получения из очереди...";
// const string QueueMessageErrorExist = "ошибка очереди...";

class Message
{
private:
    Process *_source; // указатель на процесс отправляющий сообщение
    ProcessID _destId; // процесс которому отправляется сообщение
    Command _command; // команда о том что именно делать 

    void* _data; // указатель на данные 

public:  
    Message(Process *source = nullptr, ProcessID dest = NoID, Command command = Nope, void *data = nullptr)
        : _source{source},
          _destId{dest},
          _command{command},
          _data{data}
    {}

    virtual ~Message() {};  

    // указатель на обьект отправляющий сообщение
    inline Process* source() { return _source; }
    // идентификатор обьекта принимающего сообщение
    inline ProcessID destId() { return _destId; }
    // комманда для обработки принимающим обьектом
    inline Command command() { return _command; }
    // отправляемые данные для обработки принимающим обьектом
    inline void* data() { return _data; }
};

#endif // MESSAGE_H