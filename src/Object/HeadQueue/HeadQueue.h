#ifndef HEAD_QUEUE
#define HEAD_QUEUE

#include "Object/Message/Message.h"

const uint16_t HeadQueueSize = 32; 
const uint8_t StepSizePercent = 10; 

class HeadQueue 
{
private:
    Message* _messages[HeadQueueSize] = {};
    uint32_t _head = 0,
             _tail = 0;
    bool _empty = true;
    uint32_t _delay = 0;
    Mutex _mutex;

    // возвращает true если очередь пуста
    inline bool isFull() const { return !_empty && _head == _tail; }
    // возвращает true если очередь заполнена    
    inline bool isEmpty() const { return _empty; }

public:
    // возвращает ссылку на первое сообщение без извлечения
    Message* peek();
    // добавляет сообщение
    bool send(Message *message); 
    // извлекает сообщение
    Message* receive();
    // возвращает количество сообщений находящихся в очереди
    uint32_t count();
    // возвращает длинну очереди
    inline uint32_t size() { return HeadQueueSize; }
    // возвращает процент заполнения очреди сообщениями
    inline uint8_t filled() const { return ((_tail - _head) / HeadQueueSize) * 100; }
};

#endif // HEAD_QUEUE
