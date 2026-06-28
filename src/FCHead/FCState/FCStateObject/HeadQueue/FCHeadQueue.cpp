#include "FCHeadQueue.h"

FCMessage* FCHeadQueue::peek()
{
    ScopedLock<Mutex> lock(_mutex); 
    if(isEmpty())
    {
        return nullptr;
    }
    return _messages[_tail];
} 

bool FCHeadQueue::send(FCMessage *message) 
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

FCMessage* FCHeadQueue::receive() 
{
    ScopedLock<Mutex> lock(_mutex); 
    if(isEmpty()) 
    {
        return nullptr;
    }    

    FCMessage* msg = _messages[_tail];
    _tail = (_tail + 1) % HeadQueueSize;
    _empty = (_head == _tail);

    return msg;
}

uint32_t FCHeadQueue::count()
{
    ScopedLock<Mutex> lock(_mutex); 
    if (_empty)
    { 
        return 0;
    }

    return (_head - _tail + HeadQueueSize) % HeadQueueSize;
}
