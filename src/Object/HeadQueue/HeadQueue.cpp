#include "Object/HeadQueue/HeadQueue.h"

Message* HeadQueue::peek()
{
    ScopedLock<Mutex> lock(_mutex); 
    if(isEmpty())
    {
        return nullptr;
    }
    return _messages[_tail];
} 

bool HeadQueue::send(Message *message) 
{
    ScopedLock<Mutex> lock(_mutex); 
    if (isFull()) 
    {
        return false;
    }

    _messages[_head] = message;
    _head = (_head + 1) % HeadQueueSize;
    
    _empty = false;

    return true;
}

Message* HeadQueue::receive() 
{
    ScopedLock<Mutex> lock(_mutex); 
    if(isEmpty()) 
    {
        return nullptr;
    }    

    Message* msg = _messages[_tail];
    _tail = (_tail + 1) % HeadQueueSize;
    _empty = (_head == _tail);

    return msg;
}

uint32_t HeadQueue::count()
{
    ScopedLock<Mutex> lock(_mutex); 
    if (_empty)
    { 
        return 0;
    }

    return (_head - _tail + HeadQueueSize) % HeadQueueSize;
}
