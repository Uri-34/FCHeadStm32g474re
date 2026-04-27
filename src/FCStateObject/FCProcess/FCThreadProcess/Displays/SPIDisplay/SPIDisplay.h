#ifndef SPI_DISPLAY_H
#define SPI_DISPLAY_H

#include "Bus/SPIBus/SPIBus.h"
#include "Head/Device/Driver/Driver.h"
#include "Object/Process/ThreadProcess/Displays/Display.h"
#include "Object/VisualObjects/Text/Text.h"
#include "Object/VisualObjects/Image/Image.h"
#include "Object/Process/ThreadProcess/Displays/Color/Color.h"
#include "Object/Process/ThreadProcess/Displays/SPIDisplay/Fonts/Font5x7/Font5x7.h"



struct DisplayPorts
{
    DigitalOut _cs; // вывод комманд
    DigitalOut _dc; // вывод данных
    DigitalOut _rst; // вывод перезагрузки
    DigitalOut _backLight; // вывод управления подсветкой (опционально)

    DisplayPorts(const DigitalOut &cs = PA_0, const DigitalOut &dc = PA_0, 
                 const DigitalOut &rst = PA_0, const DigitalOut &backLight = PA_0)
        : _cs{cs},
          _dc{dc},
          _rst{rst},
          _backLight{backLight}
    {}

    inline DigitalOut& cs() { return _cs; };
    inline void cs(uint8_t cs) { _cs = cs; };

    inline DigitalOut& dc() { return _dc; };
    inline void dc(uint8_t dc) { _dc = dc; };

    inline DigitalOut& rst() { return _rst; };
    inline void rst(uint8_t rst) { _rst = rst; };
    
    inline DigitalOut& backLight() { return _backLight; };
    inline void backLight(uint8_t backLight) { _backLight = backLight; };

    inline void set(uint8_t cs) { _cs = cs; }    
    inline void set(uint8_t cs, uint8_t dc) { _cs = cs; _dc = dc; }    
    inline void set(uint8_t cs, uint8_t dc, uint8_t rst) { _cs = cs; _dc = dc; _rst = rst; }    
};

class SPIDisplayData 
    : public DisplayData 
{
private:    
    SPIBus *_spi; // указатель на шину SPI
    RotateAngle _angle; // поворот дисплея 0 - по умолчаниб (портретное расположение)
    DisplayPorts _ports; // выходы дисплея для управления PA_0 - по умолчаниб

public:    
    SPIDisplayData(SPIBus* spi = nullptr, const Size2D &size = {0, 0}, const RotateAngle &angle = {ANGLE_0}, 
                   const Point2D &cursor = {0, 0}, const DisplayPorts &ports = {}, Font *font = nullptr, color_t background)
        : DisplayData(size, cursor, font, background),
          _spi{spi}, 
          _angle{angle},
          _ports{ports}
    {}

    inline RotateAngle& angle() { return _angle; } // возвращает угол поворота 
    inline DisplayPorts& ports() { return _ports; } // возвращает состояние портов
    inline SPIBus *spi() { return _spi; } // возвращает указатель на шину
};

class SPIDisplay 
    : public Display // spi дисплей работающий в отдельном потоке
{
private:
    SPIDisplayData _data; // данные дисплея 

    virtual void write(const uint8_t* bytes, size_t len) = 0; // запись массива однобайтовых последовательностей указанной длины
    virtual void write(const uint16_t* words, size_t len) = 0; // запись массива двухбайтовых последовательностей указанной длины
    virtual void write(const Rect &rect, const uint8_t* bytes, size_t len) = 0; // запись массива двухбайтовых последовательностей заявленной длины

    virtual void draw(Area2D &area, const uint16_t* image) = 0; // вывод изображения (RGB565, без сжатия)
    virtual void draw(Point2D &point, const uint16_t pixel) = 0; // рисование пикселя

    virtual void creatWindow(const Area2D &area) = 0; // определить окно вывода


    int16_t getStringWidth(const char* str);
    void drawChar(const Point2D &point, const char ch, const TextProperty &property);

public:
    SPIDisplay(const SPIDisplayData &data, 
               HeadQueue* queue, const std::string& name, 
               const ProcessID id, osPriority_t priority, rtos::Kernel::Clock::duration_u32 timeout)
        : Display(data, queue, name, id, priority, timeout),
          _data{data}
    {}   

    inline SPIDisplayData& data() { return _data; } // возвращает состояние портов
};

#endif // SPI_DISPLAY_H