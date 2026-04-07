#ifndef WINDOW_H
#define WINDOW_H

#include "Common/Common.h"
#include "Object/VisualObjects/Text/Text.h"
#include "Object/VisualObjects/Image/Image.h"
#include "Object/Process/ThreadProcess/Displays/Color/Color.h"
#include "Object/VisualObjects/Window/Frame/Border/Border.h"

struct Cursor // курсор
    : Point2D
{
    uint8_t form; // форма

    Cursor(const int16_t x = 0, const int16_t y = 0, uint8_t form = 1) 
        : Point2D(x, y)
    {}
};

class Window // окно
    : protected VisualObject 
{
private:
    Border _border; // рамка 
    color_t _background{0}; // цвет фона
    vector<VisualObject> _objects; // вектор обьектов
    Cursor _cursor; // позиция курсора - 0, 0 ; форма - 1; - по умолчаниб  

public:
    Window(const Area2D &area, const string &name, const uint32_t id, VisualObject *parent = nullptr) 
        : VisualObject(area, name, WindowID, parent)
    {}      

    inline void setBackground(color_t color) { _background = color; } // установка цвета фона
    inline color_t background() { return _background; } // возврат цвета фона

    inline void setBorder(const Border &border) { _border = border; } // установка рамки
    inline const Border& border() { return _border; } // возврат ссылки на рамку

    inline void setCursor(const Cursor &cursor) { _cursor = cursor; } // установка положения курсора
    inline const Cursor& cursor() { return _cursor; } // возврат положения курсора

    inline void operator<<(const Text &text) { _objects.push_back(text); } // добавление строки
    inline void operator<<(const Image &image) { _objects.push_back(image); } // добавление изображения
    inline void operator<<(const Window &window) { _objects.push_back(window); } // добавление окна

    virtual void show() override; // показать
    virtual void hide() override; // скрыть
};

#endif // WINDOW_H