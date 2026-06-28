#ifndef TEXT_H
#define TEXT_H

#include "Common/Common.h"
#include "Object/Process/ThreadProcess/Displays/Color/Color.h"
#include "Object/VisualObjects/VisualObject.h"



struct TextProperty
{
    Point2D _point;        // позиция
    Color _color;        // цвет
    uint8_t _scale{1};     // масштаб
};

class Text
    : public VisualObject
{
private:    
    TextProperty _property;

public:    
    Text(const Point2D &point, const string& name, const TextProperty &property, Object *parent = nullptr)
        : VisualObject(name, parent),
          _property{property}
    {}

    const TextProperty& property() { return _property; }

    virtual void show() override;
    virtual void hide() override;
};

#endif // TEXT_H
