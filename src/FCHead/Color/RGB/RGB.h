#ifndef RGB_H
#define RGB_H

#include "mbed.h"

class RGB
{
private:
    struct rgb
    {
        uint8_t _r, // red 
                _g, // green
                _b; // blue
                
    } _rgb;

public:
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) { set(r, g, b); }
    RGB(RGB &rgb) { set(rgb); }

    // инициализация покомпонентно
    void set(const uint8_t r, const uint8_t g, const uint8_t b) { _rgb = {r, g, b}; }
    // инициализация структуры
    inline void set(RGB &rgb) { set(rgb.r(), rgb.g(), rgb.b()); }

    // возврат знечения red
    inline uint8_t r() { return _rgb._r; }
    // возврат знечения green
    inline uint8_t g() { return _rgb._g; }
    // возврат знечения blue
    inline uint8_t b() { return _rgb._b; }

    inline bool operator==(RGB &rgb)
    {
        return (_rgb._r == rgb.r()) && (_rgb._g == rgb.g()) && (_rgb._b == rgb.b());
    }

    inline bool operator!=(RGB &rgb)
    {
        return (_rgb._r != rgb.r()) || (_rgb._g != rgb.g()) || (_rgb._b != rgb.b());
    }

    void operator=(RGB &rgb)
    {
        _rgb._r = rgb.r();
        _rgb._g = rgb.g();
        _rgb._b = rgb.b();
    }

};

#endif // RGB_H