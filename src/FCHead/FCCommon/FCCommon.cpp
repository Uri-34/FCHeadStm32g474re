// конвертируем rgb в cmyk
#include "FCCommon.h"

CMYK rgb2cmyk(RGB &rgb) 
{ 
    // Нормализация в диапазон [0, 1]
    float rNorm = rgb.r() / 255.0;
    float gNorm = rgb.g() / 255.0;
    float bNorm = rgb.b() / 255.0;

    // Вычисление ключевого (чёрного) компонента
    float k = 1.0 - std::max(rNorm, std::max(gNorm, bNorm));
    float c = 0, m = 0, y = 0;

    if (k != 1.0) 
    {
        // считаем и переводим в проценты
        c = (1.0 - rNorm - k) / (1.0 - k) * 100;
        m = (1.0 - gNorm - k) / (1.0 - k) * 100 ;
        y = (1.0 - bNorm - k) / (1.0 - k) * 100;
    } 

    CMYK cmyk(c, m, y, k);

    return cmyk;
}

// конвертируем cmyk в rgb 
RGB cmyk2rgb(CMYK &cmyk) 
{ 
    // Нормализация процентов (0.0 - 1.0)
    float cNorm = cmyk.c() / 100.0;
    float mNorm = cmyk.m() / 100.0;
    float yNorm = cmyk.y() / 100.0;
    float kNorm = cmyk.k() / 100.0;

    // Вычисление RGB в нормализованном виде
    float rNorm = (1.0 - cNorm) * (1.0 - kNorm);
    float gNorm = (1.0 - mNorm) * (1.0 - kNorm);
    float bNorm = (1.0 - yNorm) * (1.0 - kNorm);

    // Переводим в диапазон 0-255
    uint8_t r = static_cast<int>(std::round(rNorm * 255));
    uint8_t g = static_cast<int>(std::round(gNorm * 255));
    uint8_t b = static_cast<int>(std::round(bNorm * 255));

    // Ограничение значений 0-255
    RGB rgb(
        max<uint8_t>(0, min<uint8_t>(255, r)),
        max<uint8_t>(0, min<uint8_t>(255, g)),
        max<uint8_t>(0, min<uint8_t>(255, b))
    );

    return rgb;
}

const FCArea2D FCArea2D::intersection(const FCArea2D& area)
{
    int16_t left1 = _point._x;
    int16_t top1  = _point._y;
    int16_t right1 = left1 + _size.width();
    int16_t bottom1 = top1 + _size.height();

    int16_t left2 = area._point._x;
    int16_t top2  = area._point._y;
    int16_t right2 = left2 + _size.width();
    int16_t bottom2 = top2 + _size.height();

    // Координаты пересечения
    int16_t left   = std::max(left1, left2);
    int16_t top    = std::max(top1, top2);
    int16_t right  = std::min(right1, right2);
    int16_t bottom = std::min(bottom1, bottom2);

    // Если нет пересечения
    if (left >= right || top >= bottom) 
    {
        return FCArea2D(0, 0, 0, 0); // пустая область
    }

    return FCArea2D(left, top, right - left, bottom - top);
}
