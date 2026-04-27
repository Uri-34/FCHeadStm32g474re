#ifndef FC_MESSAGE_H
#define FC_MESSAGE_H

#include "FCProcess.h"
#include "FCCommon.h"

// // стандартные сообщения об ошибке
// const string QueueMessageErrorUpdate = "ошибка добавления в очередь...";
// const string QueueMessageErrorGet = "ошибка получения из очереди...";
// const string QueueMessageErrorExist = "ошибка очереди...";

class FCMessage
{
private:
    FCProcess *_source; // указатель на процесс отправляющий сообщение
    FCProcessID _destId; // процесс которому отправляется сообщение
    FCCommand _command; // команда о том что именно делать 

    void* _data; // указатель на данные 

public:  
    FCMessage(FCProcess *source = nullptr, FCProcessID dest = NoID, FCCommand command = Nope, void *data = nullptr)
        : _source{source},
          _destId{dest},
          _command{command},
          _data{data}
    {}

    virtual ~FCMessage() {};  

    // указатель на обьект отправляющий сообщение
    inline FCProcess* source() { return _source; }
    // идентификатор обьекта принимающего сообщение
    inline FCProcessID destId() { return _destId; }
    // комманда для обработки принимающим обьектом
    inline FCCommand command() { return _command; }
    // отправляемые данные для обработки принимающим обьектом
    inline void* data() { return _data; }
};

#endif // FC_MESSAGE_H