#include "Head/Device/Driver/Driver.h"
#include "Object/Process/ThreadProcess/Displays/SPIDisplay/SPIDisplay.h"

void SPIDisplay::write(const uint8_t* bytes, size_t length) 
{
    data().ports().dc(1);
    data().ports().cs(0);
    for (size_t count = 0; count < length; count++) 
    {
        _data.spi()->write(bytes[count]);
    }
    data().ports().cs(1);
}

void SPIDisplay::write(const uint16_t* words, size_t length) 
{
    data().ports().dc(1);
    data().ports().cs(0);
    for (size_t count = 0; count < length; count++) 
    {
        _data.spi()->write(words[count] >> 8);
        _data.spi()->write(words[count] & 0xFF);
    }
    data().ports().cs(1);
}

void SPIDisplay::draw(Point2D &point, const uint16_t word)
{
    if (point._x < 0 || point._x >= _size._width || point._y < 0 || point._y >= _size._height) 
    {
        return;
    }

    setArea(point, point);
    write(&word, 1);
}

void SPIDisplay::drawChar(const Point2D &point, const char ch, const TextProperty &property)
{
    if (!isBounded<uint16_t>(32, ch, 255) || !font) 
    {
        return;
    }    

    uint8_t glyph[5];
    memcpy(glyph, &_font[(ch - 32) * 5], 5);

    for (int8_t count = 0; count < 5; count++) 
    {
        uint8_t line = glyph[count];
        for (int8_t b = 0; b < 8; b++) {
            color_t color = (line & 0x01) ? property._color._color : property._color._bgColor;
            if (color != 0xFFFF) // не рисуем, если фон "прозрачный" 
            { 
                fill({point._x + count * property._scale, point._y + b * property._scale}, color);
            }
            line >>= 1;
        }
    }
}

void SPIDisplay::draw(Rect &rect, const uint16_t* image)
{
    if(image) 
    {
        setOutWindow(rect);
        write(image, rect.square());
    }
}
