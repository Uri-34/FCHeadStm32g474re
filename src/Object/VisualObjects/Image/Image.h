#ifndef IMAGE_H
#define IMAGE_H

#include "Object/VisualObjects/VisualObject.h" 

struct ImageProperty
{
    Area2D _area; // область вывода
    uint8_t _transparency; // прозрачность {0..256} ??? 

    ImageProperty(const Area2D &area, const uint8_t _transparency)
        : _area{area},
          _transparency{_transparency}  
    {}
};

class Image
    : public VisualObject 
{
private:    
    ImageProperty _property;
    uint16_t* _imageData; // указатель на данные 

public:    
    // Image(const string &name, const ImageProperty &property, uint16_t* imageData, Object *parent = nullptr)
    //     : VisualObject(name, parent),
    //       _property{property},
    //       _imageData{imageData}
    // {}

    Image(const Area2D &area, uint16_t* imageData, const string &name = {}, const uint8_t transparency = 0, Object *parent = nullptr)
        : VisualObject(name, NoID, parent),
          _property{area, transparency},
          _imageData{imageData}
    {}

    inline void setImage(uint16_t* imageData) { _imageData = imageData; }
    inline uint16_t* image() { return _imageData; }

    const ImageProperty& property() { return _property; }

    virtual void show() override;
    virtual void hide() override;
};

#endif // IMAGE_H
