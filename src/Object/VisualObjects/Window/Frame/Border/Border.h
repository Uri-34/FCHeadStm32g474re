#ifndef BORDER_H
#define BORDER_H

#include "Object/Process/ThreadProcess/Displays/Color/Color.h"

struct Border
{
// private:
    Color _color; // цвет окантовки
    uint8_t _thickness; // толщина в пикселях
    uint8_t _retreat; // отступ в пикселях

// public:
    Border(const Color &color = 0x0000, uint8_t thickness = 0, uint8_t retreat = 0)
        : _color{color},
          _thickness{thickness},
          _retreat{retreat}
    {}
}; 

#endif // BORDER_H