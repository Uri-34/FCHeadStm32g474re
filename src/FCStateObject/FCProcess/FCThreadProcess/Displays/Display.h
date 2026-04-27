#ifndef DISPLAY_H
#define DISPLAY_H

#include "Common/Common.h"
#include "Object/Process/ThreadProcess/Displays/SPIDisplay/Fonts/Font5x7/Font5x7.h"
#include "Object/Process/ThreadProcess/ThreadProcess.h"
#include "Object/Process/ThreadProcess/Displays/Color/Color.h"

struct DisplayData
{
    Size2D _size; // размер дисплея   
    Point2D _cursor; // курсор в позиции - 0, 0 - по умолчаниб  
    Font *_font; // инициализация указателя на шрифтовую таблицу nullptr - по умолчаниб
    color_t _background; // цвет фона

    DisplayData(const Size2D &size, const Point2D &cursor, Font *font = nullptr, color_t background)
        : _size{size}, 
          _cursor{cursor},
          _font{font},
          _background{background}
    {}
};

class Display 
    : public ThreadProcess // дисплей работающий в отдельном потоке
{
private:
    DisplayData _data; // данные дисплея 

    virtual bool init() = 0; // инициализация дисплея

    virtual void fill(Area2D &area, const color_t color) = 0; // заполнение прямоугольника

public:
    Display(const DisplayData &data, 
            HeadQueue* queue, const std::string& name, const ProcessID id, 
            osPriority_t priority, rtos::Kernel::Clock::duration_u32 timeout)
        : ThreadProcess(queue, name, id, priority, timeout),
          _data{data}
    {}      

    inline Size2D& size() { return _data._size; } // размеры экрана (меняются при повороте)
    inline void setSize(const Size2D& s) { _data._size = s; } // размеры экрана (меняются при повороте)
    inline Point2D& cursor() { return _data._cursor; } // положение курсора
    inline const Font* font() { return _data._font; } // указатель на массив шрифта
    inline void clear(Area2D &area) { fill(area, _data._background); } // стирание прямоугольника цветом фона
};

#endif // SPI_DISPLAY_H