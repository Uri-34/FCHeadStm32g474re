#ifndef FC_COMMON_H
#define FC_COMMON_H

#include <vector>
#include <algorithm>

#include <stdlib.h>

#include "mbed.h"

#define ERROR -1

// набор команд для использования их в сообщениях
enum FCCommand
{
    Nope,
    Start,
    Stop,
    Show,
    Hide,
    ChangeColor
};

//! нормализует значение value по min b max
template <typename T> 
constexpr bool contains(const T &value, const vector<T> &vec)
{
    return count(vec.begin(), vec.end(), value) > 0 ? true : false;
}

//! проверяет вхождение в диапазон от min до max
template <typename T> 
constexpr bool isBounded(const T &min, const T &value, const T &max) 
{
    if (min > max) 
    {
        return false;
    }
    
    return value < min ? false : value > max ? false : true;
}

template <typename T>
class FCRange
{
private:    
    T _min,
      _max;

public:      
    FCRange(T min, T max) 
        : _min{min}, _max{max} 
    {} 
    
    inline const T& arrange(const T &value) { return bound(_min, value, _max); }        
    inline bool inrange(const T &value) { return isBounded(_min, value, _max);} 
    inline const T& size() { return _max - _min + 1; }  
    inline const T& percent(const T &value) { return (size() / 100) * value; } 
    inline bool lessThan(const T &value) { return value < _min; }
    inline bool moreThan(const T &value) { return value > _max;; }  
};

//! нормализует значение value по min b max
template <typename T> 
constexpr const T& bound(const T &min, const T &value, const T &max)
{
    if (min > max) 
    {
        return value;
    }
    
    return value < min ? min : value > max ? max : value;
}

//! геренация случайного числа в диапазоне от min до max
//! = random()
//! = random(min)
//! = random(min, max)
template <typename T> 
constexpr const T random(const T &min = 0, const T &max = 0) 
{
    if (min > max) 
    {
        return 0;
    }

    return min + rand()%(max - min + 1);
}

//! тип Point2D 
struct FCPoint2D
{
    int16_t _x,
            _y;

    FCPoint2D(const int16_t x = 0, const int16_t y = 0) 
        : _x{x}, 
          _y{y} 
    {}

    inline bool operator>(const FCPoint2D &point) { return (_x > point._x || _y > point._y); }
    inline bool operator>=(const FCPoint2D &point) { return (_x >= point._x || _y >= point._y); }
    inline bool operator<(const FCPoint2D &point) { return (_x < point._x || _y < point._y); }
    inline bool operator<=(const FCPoint2D &point) { return (_x <= point._x || _y <= point._y); }
};

//! тип Point3D 
struct FCPoint3D 
    : public FCPoint2D 
{
    int16_t _z;

    FCPoint3D(const int16_t x = 0, const int16_t y = 0, const int16_t z = 0) 
        : Point2D(x, y), _z{z} 
    {}

    inline bool operator>(const Point3D &point) { return (_z > point._z || _x > point._x || _y > point._y); }
    inline bool operator>=(const Point3D &point) { return (_z >= point._z || _x >= point._x || _y >= point._y); }
    inline bool operator<(const Point3D &point) { return (_z < point._z || _x < point._x || _y < point._y); }
    inline bool operator<=(const Point3D &point) { return (_z <= point._z || _x <= point._x || _y <= point._y); }
};

//! тип Rect 
struct FCRect
{
    FCPoint2D _leftUp,
            _rightDown;

    FCRect(const Point2D leftUp = {0, 0}, const FCPoint2D rightDown = {0, 0}) 
        : _leftUp{leftUp}, 
          _rightDown{rightDown} 
    {}

    inline bool operator>(FCRect &rect) { return square() > rect.square(); } 
    inline bool operator>=(FCRect &rect) { return square() >= rect.square(); } 
    inline bool operator<(FCRect &rect) { return square() < rect.square(); } 
    inline bool operator<=(FCRect &rect) { return square() <= rect.square(); } 

    inline int16_t height() { return abs(_rightDown._y - _leftUp._y); } 
    inline int16_t width() { return abs(_rightDown._x - _leftUp._x); } 

    inline int32_t square() { return abs(height() * width()); } 
};

//! тип Cube 
struct FCCube
{
    FCPoint2D  _leftUp,
             _rightDown;
    uint16_t _z;

    FCCube(const FCPoint2D leftUp, const FCPoint2D rightDown, uint16_t z) 
        : _leftUp{leftUp}, 
          _rightDown{rightDown},
          _z{z}
    {}

    inline bool operator>(Cube &cube) { return volume() > cube.volume(); }
    inline bool operator>=(Cube &cube) { return volume() >= cube.volume(); }
    inline bool operator<(Cube &cube) { return volume() < cube.volume(); }
    inline bool operator<=(Cube &cube) { return volume() <= cube.volume(); }

    inline uint32_t volume() { return abs((_rightDown._x - _leftUp._x) * (_rightDown._y - _leftUp._y) * _z); }
};

//! тип Size2D 
class FCSize2D
{
private:    
    int16_t _width,
            _height;

public:
    FCSize2D(const int16_t width = 0, const int16_t height = 0) 
        : _width{width}, 
          _height{height} 
    {}

    inline bool operator>(const FCSize2D &size) { return (_width > size._width || _height > size._height); }
    inline bool operator>=(const FCSize2D &size) { return (_width >= size._width || _height >= size._height); }
    inline bool operator<(const FCSize2D &size) { return (_width < size._width || _height < size._height); }
    inline bool operator<=(const FCSize2D &size) { return (_width <= size._width || _height <= size._height); }

    inline int16_t width() { return _width; }
    inline int16_t height() { return _height; }

    inline int16_t length() { return _height + _width; }

};

//! тип Size3D 
struct FCSize3D
    : public FCSize2D
{
    int16_t _z;

    FCSize3D(const int16_t width = 0, const int16_t height = 0, const int16_t z = 0) 
        : FCSize2D(width, height),
          _z{z}   
    {}

    inline bool operator>(FCSize3D &size) { return (_z > size._z || _width > size._width || _height > size._height); }
    inline bool operator>=(FCSize3D &size) { return (_z >= size._z || _width >= size._width || _height >= size._height); }
    inline bool operator<(FCSize3D &size) { return (_z < size._z || _width < size._width || _height < size._height); }
    inline bool operator<=(FCSize3D &size) { return (_z <= size._z || _width <= size._width || _height <= size._height); }
};

// область с началом _point и размером _size
class FCArea2D 
{
private:    
    FCPoint2D _point;
    FCSize2D _size; 

public:    
    FCArea2D(const FCPoint2D &point = FCPoint2D(), const FCSize2D &size = FCSize2D())
        : _point{point},
          _size{size} 
    {}

    FCArea2D(int16_t x, int16_t y, int16_t width, int16_t height)
        : _point{x, y},
          _size{width, height} 
    {}

    inline const FCPoint2D& point() const { return _point; }

    inline FCSize2D& size() const { return _size; }
    inline void setSize(const FCSize2D &size) { _size = size; }

    inline bool isBound(FCPoint2D &point)
    {
        return isBounded<int16_t>(_point._x, point._x, _point._x + _size._width) 
               && isBounded<int16_t>(_point._y, point._y, _point._y + _size._height);
    }

    inline bool isBound(int16_t x, int16_t y)
    {
        return isBounded<int16_t>(_point._x, x, _point._x + _size._width) 
               && isBounded<int16_t>(_point._y, y, _point._y + _size._height);
    }

    inline bool isEmpty()
    {
        return _size.width() == 0 && _size.height() == 0;
    }

    const Area2D intersection(const Area2D& area);
};

// область с началом _point и размером _size
struct FCArea3D
    : public FCPoint3D
{
    FCSize3D _size;

    Area3D(const FCPoint3D &point = FCPoint3D(), const FCSize3D &size = FCSize3D())
        : FCPoint3D{point},
          _size{size}
    {}

    inline bool isIncluded(const FCPoint3D &point)
    {
        return isBounded<int16_t>(_x, point._x, _x +_size._width)
               && isBounded<int16_t>(_y, point._y, _y +_size._height)
               && isBounded<int16_t>(_z, point._z, _z +_size._z);
    }
};

typedef pair<uint8_t, bool> Uint8_tBoolPair;
typedef vector<Uint8_tBoolPair> Uint8_tBoolPairList;

#include "CMYK.h"
#include "RGB.h"

// конвертируем rgb в cmyk
CMYK rgb2cmyk(RGB &rgb);
// конвертируем cmyk в rgb 
RGB cmyk2rgb(CMYK &cmyk);

#endif // FC_COMMON_H
