#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <vector>

#include "mbed.h"

#include "Common/Common.h"
#include "Color/RGB/RGB.h"

union Pixel 
{
    struct __attribute__((packed)) 
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a; // unused
    };
    uint32_t hex;
};

class NeoPixel : DigitalOut 
{
private:
    void byte(uint32_t b);   
    int _npixels;
    Pixel *_pixels;
    
public:
    NeoPixel(PinName, int );
    void show(void);
    void hide(void);
    // устанавливает цвет color одного диода в позиции position
    inline void setColor(const uint32_t color, const int position)
    {
        (_pixels + bound(0, position, _npixels))->hex = color;
    }
    // устанавливает цвет color одного диода в позиции position
    void setColor(RGB &rgb, int position)
    {
        Pixel pixel = {rgb.b(), rgb.g(), rgb.r()};
        (_pixels + bound(0, position, _npixels))->hex = pixel.hex;
    }

    // устанавливает цвет ленты диодов в соответствии с переданным вектором RGB()
    void setColor(vector<RGB> &vRgb);    
    inline uint8_t size() { return _npixels; }
};

#endif /* NEOPIXEL_H */
