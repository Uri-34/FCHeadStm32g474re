#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <algorithm>

#include <stdlib.h>

#include "mbed.h"

#define ERROR -1

// набор команд для использования их в сообщениях
enum Command
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
class Range
{
private:    
    T _min,
      _max;

public:      
    Range(T min, T max) 
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
struct Point2D
{
    int16_t _x,
            _y;

    Point2D(const int16_t x = 0, const int16_t y = 0) 
        : _x{x}, 
          _y{y} 
    {}

    inline bool operator>(const Point2D &point) { return (_x > point._x || _y > point._y); }
    inline bool operator>=(const Point2D &point) { return (_x >= point._x || _y >= point._y); }
    inline bool operator<(const Point2D &point) { return (_x < point._x || _y < point._y); }
    inline bool operator<=(const Point2D &point) { return (_x <= point._x || _y <= point._y); }
};

//! тип Point3D 
struct Point3D 
    : public Point2D 
{
    int16_t _z;

    Point3D(const int16_t x = 0, const int16_t y = 0, const int16_t z = 0) 
        : Point2D(x, y), _z{z} 
    {}

    inline bool operator>(const Point3D &point) { return (_z > point._z || _x > point._x || _y > point._y); }
    inline bool operator>=(const Point3D &point) { return (_z >= point._z || _x >= point._x || _y >= point._y); }
    inline bool operator<(const Point3D &point) { return (_z < point._z || _x < point._x || _y < point._y); }
    inline bool operator<=(const Point3D &point) { return (_z <= point._z || _x <= point._x || _y <= point._y); }
};

//! тип Rect 
struct Rect
{
    Point2D _leftUp,
            _rightDown;

    Rect(const Point2D leftUp = {0, 0}, const Point2D rightDown = {0, 0}) 
        : _leftUp{leftUp}, 
          _rightDown{rightDown} 
    {}

    inline bool operator>(Rect &rect) { return square() > rect.square(); } 
    inline bool operator>=(Rect &rect) { return square() >= rect.square(); } 
    inline bool operator<(Rect &rect) { return square() < rect.square(); } 
    inline bool operator<=(Rect &rect) { return square() <= rect.square(); } 

    inline int16_t height() { return abs(_rightDown._y - _leftUp._y); } 
    inline int16_t width() { return abs(_rightDown._x - _leftUp._x); } 

    inline int32_t square() { return abs(height() * width()); } 
};

//! тип Cube 
struct Cube
{
    Point2D  _leftUp,
             _rightDown;
    uint16_t _z;

    Cube(const Point2D leftUp, const Point2D rightDown, uint16_t z) 
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
class Size2D
{
private:    
    int16_t _width,
            _height;

public:
    Size2D(const int16_t width = 0, const int16_t height = 0) 
        : _width{width}, 
          _height{height} 
    {}

    inline bool operator>(const Size2D &size) { return (_width > size._width || _height > size._height); }
    inline bool operator>=(const Size2D &size) { return (_width >= size._width || _height >= size._height); }
    inline bool operator<(const Size2D &size) { return (_width < size._width || _height < size._height); }
    inline bool operator<=(const Size2D &size) { return (_width <= size._width || _height <= size._height); }

    inline int16_t width() { return _width; }
    inline int16_t height() { return _height; }

    inline int16_t length() { return _height + _width; }

};

//! тип Size3D 
struct Size3D
    : public Size2D
{
    int16_t _z;

    Size3D(const int16_t width = 0, const int16_t height = 0, const int16_t z = 0) 
        : Size2D(width, height),
          _z{z}   
    {}

    inline bool operator>(Size3D &size) { return (_z > size._z || _width > size._width || _height > size._height); }
    inline bool operator>=(Size3D &size) { return (_z >= size._z || _width >= size._width || _height >= size._height); }
    inline bool operator<(Size3D &size) { return (_z < size._z || _width < size._width || _height < size._height); }
    inline bool operator<=(Size3D &size) { return (_z <= size._z || _width <= size._width || _height <= size._height); }
};

// область с началом _point и размером _size
class Area2D 
{
private:    
    Point2D _point;
    Size2D _size; 

public:    
    Area2D(const Point2D &point = Point2D(), const Size2D &size = Size2D())
        : _point{point},
          _size{size} 
    {}

    Area2D(int16_t x, int16_t y, int16_t width, int16_t height)
        : _point{x, y},
          _size{width, height} 
    {}

    inline const Point2D& point() const { return _point; }

    inline Size2D& size() const { return _size; }
    inline void setSize(const Size2D &size) { _size = size; }

    inline bool isBound(Point2D &point)
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
struct Area3D
    : public Point3D
{
    Size3D _size;

    Area3D(const Point3D &point = Point3D(), const Size3D &size = Size3D())
        : Point3D{point},
          _size{size}
    {}

    inline bool isIncluded(const Point3D &point)
    {
        return isBounded<int16_t>(_x, point._x, _x +_size._width)
               && isBounded<int16_t>(_y, point._y, _y +_size._height)
               && isBounded<int16_t>(_z, point._z, _z +_size._z);
    }
};

typedef pair<uint8_t, bool> Uint8_tBoolPair;
typedef vector<Uint8_tBoolPair> Uint8_tBoolPairList;

#include "Color/CMYK/CMYK.h"
#include "Color/RGB/RGB.h"

// конвертируем rgb в cmyk
CMYK rgb2cmyk(RGB &rgb);
// конвертируем cmyk в rgb 
RGB cmyk2rgb(CMYK &cmyk);

#endif // COMMON_H