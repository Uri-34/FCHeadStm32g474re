#ifndef CMYK_H
#define CMYK_H

#include "mbed.h"

class CMYK
{
private:
    struct cmyk
    { 
        float  _c, // cyan
               _m, // magenta
               _y, // yellow
               _k; // черный компонент
    } _cmyk;

public:
    // инициализация покомпонентно
    inline void set(const float c, const float m, const float y, float k) { _cmyk = {c, m, y, k}; }
    // инициализация структуры
    inline void set(CMYK &cmyk) { _cmyk = {cmyk.c(), cmyk.m(), cmyk.y(), cmyk.k()}; }

    // перегрузка оператора ==
    inline bool operator ==(CMYK cmyk)
    {
        return(_cmyk._c == cmyk.c() && _cmyk._m == cmyk.m() && _cmyk._y == cmyk.y() && _cmyk._k == cmyk.k()); 
    }

    // перегрузка оператора !=
    inline bool operator !=(CMYK cmyk)
    {
        return(_cmyk._c != cmyk.c() || _cmyk._m != cmyk.m() || _cmyk._y != cmyk.y() || _cmyk._k != cmyk.k()); 
    }

    // перегрузка оператора =
    CMYK operator =(CMYK cmyk)
    {
        set(cmyk);
        
        return *this;
    }

    // возврат знечения cyan
    inline float c() { return _cmyk._c; }
    // возврат знечения magenta
    inline float m() { return _cmyk._m; }
    // возврат знечения yellow
    inline float y() { return _cmyk._y; }
    // возврат знечения k
    inline float k() { return _cmyk._k; }

    // CMYK(float c = 0, float m = 0, float y = 0, float k = 0) { set(c, m, y, k); }
    CMYK(const float c = 0, const float m = 0, const float y = 0, const float k = 0) { set(c, m, y, k); }
    CMYK(CMYK &cmyk) { set(cmyk); }
};

#endif // CMYK_H