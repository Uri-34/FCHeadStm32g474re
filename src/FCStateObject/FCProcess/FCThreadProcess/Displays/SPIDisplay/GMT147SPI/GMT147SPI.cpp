#include "Object/Process/ThreadProcess/Displays/SPIDisplay/GMT147SPI/GMT147SPI.h"

GMT147SPI::GMT147SPI(SPIBus *spi, HeadQueue *queue, Kernel::Clock::duration_u32 delay)
    : SPIDisplay({spi, {GMT147SPI_WIDTH, GMT147SPI_HEIGHT}, ANGLE_0, {0, 0}, {PB_0, PB_1, PB_2, PB_3}, Font5х7},
                 queue, "gmt147spi", GMT147SPIID, osPriorityNormal, delay)
{
    set(init() ? Ready : !Ready); 

    if(is(Ready)) 
    {
        set(Started);
    }    
}

void GMT147SPI::reset() 
{
    data().ports().rst(0);
    ThisThread::sleep_for(10ms);
    
    data().ports().rst(1);
    ThisThread::sleep_for(120ms);
}

bool GMT147SPI::init()
{
    reset();

    sleep(10ms);

    command(SWRESET);
    sleep(150ms);

    command(SLPOUT); 
    sleep(10ms);

    // Установка цветового режима: 16-bit/pixel
    command(COLMOD, (uint8_t[]){RGB565}, 1); // 16 bit per pixel (MCU & SPI)

    // Установка направления памяти (по умолчанию)
    command(MADCTL, (uint8_t[]){NU}, 1); // MADCTL

    // Установка окна по умолчанию
    command(CASET, (uint8_t[]){0x00, 0x00, 0x00, 0xAD}, 4); // 0 to 172-1 (0xAD)
    command(RASET, (uint8_t[]){0x00, 0x00, 0x01, 0x3F}, 4); // 0 to 320-1 (0x13F)

    // Включение дисплея
    command(INVON); // опционально (инверсия)
    command(DISPON); // DISPON

    sleep(100ms);

    Area2D all(Point2D(0, 0), Size2D(data()._size.width(), data()._size.height()));
    clear(all);
    return true;
}

void GMT147SPI::command(const Commands &command) 
{
    data().ports().dc(0);
    data().ports().cs(0);
    write(command);
    data().ports().cs(1);
}

void GMT147SPI::data(uint8_t d) 
{
    data().ports().dc(1);
    data().ports().cs(0);
    write(d);
    data().ports().cs(1);
}

void GMT147SPI::reset() 
{
    if(data().ports().rst().is_connected()) 
    {
        data().ports().rst(0);
        ThisThread::sleep_for(10ms);
        data().ports().rst(1);
        ThisThread::sleep_for(120ms);
    } 
    else 
    {
        command(SWRESET); // программный рестарт
        ThisThread::sleep_for(150ms);
    }
}

void GMT147SPI::setAngle(const RotateAngle &angle)
{
    _angle = angle;
    switch(_angle)
    {
        case ANGLE_0:
        case ANGLE_180:
            _area = _source;
        break; 
        case ANGLE_90: 
        case ANGLE_270:
            _area = {_source._size.height(), _source._size.width()};
        break; 
    }
}

void GMT147SPI::rotate(const DisplayAngle &angle)
{  
    uint8_t madctl = angle;

    switch(angle)
    {
        case ANGLE_0:
        case ANGLE_180:
            size({GMT147SPI_WIDTH, GMT147SPI_HEIGHT});
        break;

        case ANGLE_90:
        case ANGLE_270:
            size({GMT147SPI_HEIGHT, GMT147SPI_WIDTH});
        break;

    }
    write(MADCTL, &madctl, 1);
}

void GMT147SPI::write(const Commands &cmd, uint8_t* pixels, size_t length) 
{
    command(cmd);
    if (pixels && length > 0) 
    {
        write(pixels, length);
    }
}

void GMT147SPI::operator<<(Text &text)
{
    if(!text.name().c_str())  
    {
        return;
    }        
    
    Point2D p = text._point; 
    while (*text._str) 
    {
        if (isBounded<uint16_t>(32, *text._str, 255))
        {
            draw({p._x, p._y}, *text._str);
            p._x += 6 * text._property._scale;
        }
        
        if (p._x >= _size._width) 
        {
            p._x = 0;
            p._y += 8 * text._property._scale;
            if(p._y >= _size._height) 
            {
                p._y = 0;
            }
        }
        text._str++;
    }
}

void GMT147SPI::operator<<(const Image &image)
{
    if(!text._str)  
    {
        return;
    }        
    
    Point2D p = text._point; 
    while (*text._str) 
    {
        if (isBounded<uint16_t>(32, *text._str, 255))
        {
            draw({p._x, p._y}, *text._str);
            p._x += 6 * text._property._scale;
        }
        
        if (p._x >= _size._width) 
        {
            p._x = 0;
            p._y += 8 * text._property._scale;
            if(p._y >= _size._height) 
            {
                p._y = 0;
            }
        }
        text._str++;
    }
}

void GMT147SPI::setArea(Area2D &area)
{
    if(area._size.height() && area._size.width()) 
    {
        return;
    }

    command(CASET);
    data().ports().set(0, 1);
        write(bound<uint16_t>(0, area._x, size().width() - 1));
        write(bound<uint16_t>(0, area._y, size().height() - 1));
    data().ports().set(1);
    
    command(RASET);
    data().ports().set(0, 1);
        write(bound<uint16_t>(0, area._x, size().width() - 1));
        write(bound<uint16_t>(0, area._y, size().height() - 1));
    data().ports().set(1);
    
    command(RAMWR);
}

void GMT147SPI::fill(Area2D &area, const color_t color)
{

    if(area._size.width() <= 0 || area._size.height() <= 0) 
    {
        return;
    }

    int16_t x2 = area._x + area._size.width() - 1;
    int16_t y2 = area._y + area._size.height() - 1;

    if (area._x < 0)
    { 
        area._x = 0;
    }

    if (y < 0) y = 0;
    if (x2 >= _width) x2 = _width - 1;
    if (y2 >= _height) y2 = _height - 1;

    setArea(x, y, x2, y2);

    _dc = 1;
    _cs = 0;
    for(int32_t i = 0; i < (int32_t)w * h; i++) 
    {
        write(color);
    }
    _cs = 1;
}

void GMT147SPI::rotate(const DisplayAngle &angle) 
{
    (angle == ANGLE_0 || angle == ANGLE_180) ? data().size().set(172, 320) : size().set(320, 172);

    uint8_t madctl = angle;

    write(MADCTL, &madctl, 1);
}

void GMT147SPI::handle(Message *message)
{
    switch(message->source()->id())
    {
        case MixerID:
        {
            static uint32_t cnt = 0;

            // обрабатываем данные полученные от миксера
            MixerData *data = static_cast<MixerData*>(static_cast<Message*>(message)->data()); 
            // CMYK cmyk = data->cmyk();
            // Msg(name() + " <- " + message->source()->name() + ": c=%d m=%d y=%d k=%d : получено - %d",
            //                                                        static_cast<int>(data->cmyk().c()),
            //                                                             static_cast<int>(data->cmyk().m()),
            //                                                                  static_cast<int>(data->cmyk().y()),
            //                                                                        static_cast<int>(data->cmyk().k()),
                                                                                    
            //                                                                                        cnt);
            cnt++;                  
            delete data;                                           
            delete message;
        }
        break;    
        case ConditionerID:
        {
            static uint32_t cnt = 0;

            // обрабатываем полученные данные от кондиционера
            ConditionerData *data = static_cast<ConditionerData*>(static_cast<Message*>(message)->data());
            // Msg(name() + " <- " + message->source()->name() + " : температура = %d скорость вращения = %d : полученно - %d", 
            //                                                                   data->temperature(), 
            //                                                                                          data->speed(),
            //                                                                                                             cnt);
            cnt++;        
            delete data;
            delete message;           
        }
        break;        
        case IndicatorID:
        {
            static uint32_t cnt = 0;

            // обрабатываем сообщение от сканера
            IndicatorData *data = static_cast<IndicatorData*>(static_cast<Message*>(message)->data());
            // Msg(name() + " <- " + message->source()->name() + ": количество светодиодов=%d : полученно - %d", 
            //                                                                             data->leds(),
            //                                                                                              cnt); 
            cnt++;        
            delete data;                                                                      
            delete message;           
        }
        break;    
        case LaserScanerID:
        {
            static uint32_t cnt = 0;

            // обрабатываем сообщение от сканера
            LaserScannerData *data = static_cast<LaserScannerData*>(static_cast<Message*>(message)->data());
            // Msg(name() + " <- " + message->source()->name() + ": data=%d : получено - %d", 
            //                                                           static_cast<int>(data->value()),
            //                                                                           cnt);
            cnt++;                                              
            delete data;
            delete message;           
        }
        break;    
        default:; 
    }

    // вызываем обработчик дисплея 
    // lv_timer_handler();
    ThisThread::sleep_for(5ms);
}
