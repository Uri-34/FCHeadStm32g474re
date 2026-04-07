#include "Object/VisualObjects/VisualObject.h"

void VisualBuffer::out(Point2D &point, uint8_t byte)
{
    if(!isBound(point))
    {
        return;
    }

    memmove(offset(point), &byte, 1);        
}

void VisualBuffer::out(Point2D &point, uint8_t *bytes, uint16_t length)
{
    if(!isBound(point))
    {
        return;
    }

    memmove(offset(point), bytes, length);        
}

VisualObject::VisualObject(const Area2D &area, const string &name, const uint32_t id, VisualObject *parent = nullptr)
    : Object(name, id, parent),
        _align{CENTER_MIDDLE},
        _point{area.point()},
        _buffer{VisualBuffer(area.size())},
        _transparency{0}
{}

void VisualObject::out() 
{
    VisualObject *visualParent = (VisualObject*)parent();
    if(visualParent)
    {
        // отрисовка в буфер владельца
        for(int16_t count = 0; count < size().height(); count++)
        {
            _buffer.out();
            visualParent->_buffer._buffer + (), 
                    _buffer + (count * visualParent->_buffer._area._size), visualParent->_buffer._area._size, size);        
        }        
    } 
    else
    {
        // отрисовка в буфер дисплея
        for(int16_t count = 0; count < area._size._height; count++)
        {
            memmove(dest, _buffer, size()/8);        
        }        
    }

    // memmove(dest, _buffer, size());
}
