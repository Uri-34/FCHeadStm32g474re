// #ifndef HEAD_QUEUE_H
// #define HEAD_QUEUE_H

// #include "mbed.h"

// #include "Object/Message/Message.h"

// class HeadQueue
//     : private Queue<Message, 16>
// {
// public:
//     // отправка сообщения в очередь 
//     inline bool send(Message* message, uint8_t priority = 0) { return try_put(message, priority); }
//     // получение сообщения из очереди 
//     Message* receive();
//     // возвращает количество сообщений находящихся в очереди
//     inline uint32_t count() { return Queue::count(); }
// };

// #endif // HEAD_QUEUE_H