#ifndef COLOR_H
#define COLOR_H

#include "mbed.h"

using color_t = uint16_t; // RGB565

struct Color
{
    color_t   _color, // цвет
              _bgColor; // фон, 0xFFFF - прозрачный 
    
    Color(const color_t color = 0xFFFF, const color_t bgColor = 0xFFFF)
        : _color{color},
          _bgColor{bgColor}
    {}      
};

#endif // COLOR_H
