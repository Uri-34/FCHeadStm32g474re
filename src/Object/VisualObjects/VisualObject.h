#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "Object/Object.h"

enum Align // константы выравнивания
{ 
    LEFT_UP,     CENTER_UP,     RIGHT_UP,
    LEFT_MIDDLE, CENTER_MIDDLE, RIGHT_MIDDLE,
    LEFT_DOWN,   CENTER_DOWN,   RIGHT_DOWN,
};

struct VisualBuffer
{
private:
    Size2D _size; // размер буфера в пикселях 
    uint8_t *_buffer; // указатель на буффер вывода

    // вычисление смешения в буфере относительно начала
    inline uint8_t* offset(const Point2D &point) 
    {
        uint16_t pos = position(point);
        return pos != ERROR ? _buffer + pos : nullptr;
    }

    // максимально возможное смешение в буфере относительно начала
    inline int16_t end() { return (_size.width() * _size.height()) - 1; }

    // проверка на попадание точки в размер буфера
    inline bool isBound(const Point2D &point)  
    { 
        return isBounded<int16_t>(0, point._x, _size.width())
               && isBounded<int16_t>(0, point._y, _size.height());
    }

    // вычисление позиции точки относительно размера если точка за границами возвращает -1
    int16_t position(const Point2D &point)
    { 
        uint16_t result = _size.width() * (point._y - 1) + point._x;
        return isBounded<int16_t>(0, result, end()) ? result : ERROR;
    }

public:    
    VisualBuffer(const Size2D &size = Size2D{})
    {
        _buffer = new uint8_t[_size.length()];
    }

    // возвращает размер 
    inline Size2D size() { return _size; }
    // вывод одного байта
    void out(Point2D &point, uint8_t byte); 
    // вывод последовательности байтов длиной length
    void out(Point2D &point, uint8_t *bytes, uint16_t length);   
};

class VisualObject
    : public Object
{
private:
    Point2D _point; // область вывода 
    VisualBuffer _buffer; // буффер вывода
    Align _align; // выравнивание
    uint8_t _transparency; // прозрачность

    vector<Area2D> _areas; // вектор измененных областей

    void out(); // попиксельное отображение буфера в буфер владельца
    
    void _alignTo() 
    {
        switch(_align)
        {
            case LEFT_UP:
                _buffer._area = Area2D(0, 0);
            break;
            case CENTER_UP:
                _buffer._area._point._y = 0;
                _buffer._area._point._x = 0;
            break;
            case RIGHT_UP:
            break;
            case LEFT_MIDDLE:
            break;
            case CENTER_MIDDLE:
            break;
            case RIGHT_MIDDLE:
            break;
            case LEFT_DOWN:
                _buffer._area = Area2D(0, _buffer._area.);
            break;
            case CENTER_DOWN:
            break;
            case RIGHT_DOWN:
            break;
        }    
    }

public:    
    VisualObject(const Area2D &area, const string &name, const uint32_t id, VisualObject *parent = nullptr);

    inline const Point2D& point() const { return _point; } // точка начала
    inline void setPoint(const Point2D& point) { _point = point; } // установить точки начала 
    
    inline Size2D& size() const { return _buffer.size(); } // размер виртуальной области
    inline void setSize(const Size2D& size) { _buffer.setSize(size); } // установить размер виртуальной области 

    inline Align& align() { return _align; } // выравнивание
    inline void setAlign(const Align align) { _align = align; } // установить выравнивание 

    inline uint8_t transparency() { return _transparency; } // прозрачность
    inline void setTransparency(uint8_t transparency) { _transparency = bound<uint8_t>(0, transparency, 255); } // установить прозрачность

    virtual void show() = 0; // показать 
    virtual void hide() = 0; // скрыть
};

#endif // VISUAL_OBJECT_H
