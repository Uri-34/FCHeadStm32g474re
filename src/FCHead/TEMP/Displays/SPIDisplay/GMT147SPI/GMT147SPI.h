#ifndef GMT147SPI_H
#define GMT147SPI_H

#include "Object/Process/ThreadProcess/Displays/SPIDisplay/SPIDisplay.h"
#include "Object/Process/ThreadProcess/Conditioner/Conditioner.h"
#include "Object/Process/ThreadProcess/Mixer/Mixer.h"
#include "Object/Process/ThreadProcess/LaserScaner/LaserScaner.h"
#include "Object/Process/ThreadProcess/LedIndicator/LedIndicator.h"
#include "Object/Process/ThreadProcess/Displays/SPIDisplay/Fonts/Font5x7/Font5x7.h"

// значек миксера 
const uint8_t mixerMap[] = 
{
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x0f, 0x00, 0x00, 0x00, 
  0x19, 0x80, 0x00, 0x00, 
  0x10, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x10, 0x80, 0x00, 0x00, 
  0x19, 0x80, 0x00, 0x00, 
  0x0f, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x80, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x04, 0x20, 0x00, 
  0x00, 0x04, 0x20, 0x00, 
  0x00, 0x04, 0x20, 0x00, 
  0x00, 0x04, 0x20, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x01, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xf0, 
  0x00, 0x00, 0x01, 0x98, 
  0x00, 0x00, 0x01, 0x08, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0x08, 
  0x00, 0x00, 0x01, 0x98, 
  0x00, 0x00, 0x00, 0xf0, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
};

// значек кондиционера 
const uint8_t conditionerMap[] = 
{
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0xe0, 0x00, 
  0x00, 0x38, 0x1c, 0x00, 
  0x00, 0x60, 0x02, 0x00, 
  0x00, 0x80, 0x01, 0x80, 
  0x01, 0x00, 0x00, 0xe0, 
  0x02, 0x02, 0x00, 0xb0, 
  0x06, 0x03, 0x00, 0xe0, 
  0x04, 0x07, 0x90, 0x40, 
  0x00, 0x04, 0x98, 0x00, 
  0x1c, 0x0c, 0x6c, 0x10, 
  0x16, 0x18, 0x64, 0x10, 
  0x1c, 0x30, 0x64, 0x10, 
  0x08, 0x22, 0x22, 0x10, 
  0x08, 0x42, 0xa2, 0x10, 
  0x08, 0x42, 0x22, 0x10, 
  0x04, 0x54, 0x26, 0x30, 
  0x04, 0x24, 0x6c, 0x20, 
  0x04, 0x20, 0x70, 0x20, 
  0x02, 0x1f, 0x80, 0x40, 
  0x01, 0x06, 0x00, 0x80, 
  0x00, 0x80, 0x01, 0x00, 
  0x00, 0x40, 0x02, 0x00, 
  0x00, 0x3b, 0xdc, 0x00, 
  0x00, 0x02, 0x70, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
};

const int16_t GMT147SPI_WIDTH = 172;
const int16_t GMT147SPI_HEIGHT = 320;

enum Commands
{
// Установки
  NU = 0x00, // ничего
  GAMSET = 0x26, // выбор таблицы гаммы (Gamma Set).
  INVOFF = 0x20, // отключить инверсию цвета.
  INVON = 0x21, // включить инверсию цвета.
  RAMCTRL = 0xB0, // управление RAM (опционально, зависит от версии ST7789).
  RGBCTRL = 0xB1, // управление форматом RGB (часто не используется).
  PORCTRL = 0xB3, // управление частотой кадров (Partial Mode On + Frame Rate).
  IDMOFF = 0xB6, // выключить idle mode.
  IDMON = 0xB7, // включить idle mode.
  PWCTRL1 = 0xC0, // управление питанием (Power Control 1).
  PWCTRL2 = 0xC1, // power Control 2.
  PWCTRL3 = 0xC2, // power Control 3.
  PWCTRL4 = 0xC3, // power Control 4.
  PWCTRL5 = 0xC4, // power Control 5.
  VMCTRL1 = 0xC5, // управление напряжением (VCOM Control 1).
  VCOMS = 0xC6, // установка напряжения VCOM (VCOM Setting).
  VCOMVR = 0xC7, // диапазон VCOM (VCOM Voltage Range).
  NVMPKEY = 0xD0, // ключ доступа к NVM (если есть).
  PGAMCTRL = 0xE0, // настройка положительной гаммы (Positive Gamma Control). Принимает 14-16 параметров.
  NGAMCTRL = 0xE1, // настройка отрицательной гаммы (Negative Gamma Control).

// Комманды
  SWRESET = 0x01, // программный сброс (Software Reset). Обязательно после питания. 
  SLPIN = 0x10, // вход в спящий режим (Sleep In).
  SLPOUT = 0x11, // выход из спящего режима (Sleep Out). Требует задержки ~120 мс перед следующей командой.
  DISPON = 0x29, // включить дисплей (Display On).
  DISPOFF = 0x28, // выключить дисплей (Display Off).
  CASET = 0x2A, // установка горизонтального окна (Column Address Set). Принимает Xstart, Xend (по 2 байта каждый).
  RASET = 0x2B, // установка вертикального окна (Row Address Set). Принимает Ystart, Yend.
  RAMWR = 0x2C, // запись в память дисплея (Memory Write). После этой команды идут пиксельные данные RGB565.
  RAMRD = 0x2E, // чтение из памяти дисплея (Memory Read). Редко используется в SPI-режиме.
  MADCTL = 0x36, // Управление ориентацией и порядком пикселей (Memory Access Control). 
                  // Определяет поворот экрана, RGB/BGR, вертикаль/горизонталь.
  COLMOD = 0x3A, // Установка цветового формата (Interface Pixel Format).                
  
  RGB565 = 0x55, // Обычно 0x55 — 16 бит на пиксель (RGB565)

// Частичное обновление
  VSCSAD = 0x37, // вертикальное смещение начала отображения (Vertical Scroll Start Address). Для аппаратного скролла.
  TCRTL = 0x38, // управление Tearing Effect (для синхронизации с кадром).
  TEON = 0x39, // включить Tearing Effect.
  TCTRL = 0x35, // управление режимом Tearing Effect.
  VSCRDEF = 0x33, // определение области вертикального скролла.  

// Режимы
  RDDID = 0x04, // чтение ID дисплея (Read Display ID).
  RDDPM = 0x09, // чение статуса питания (Read Display Power Mode).
  RDDMADCTL = 0x0A, // чтение MADCTL.
  RDDCOLMOD = 0x0B, // чтение COLMOD.
  RDDIM = 0x0C, // чтение Image Mode.
  RDDSM = 0x0D, // чтение Signal Mode.
  RDDSDR = 0x0F, // чтение Self-Diagnostic Result.
  RDID1 = 0xDA, // чтение ID1 (производитель).
  RDID2 = 0xDB, // чтение ID2.
  RDID3 = 0xDC, // чтение ID3.
  RAMCTRL = 0xDE, // чтение RAM control (если поддерживается).
  RGBCTRL = 0xDF // чтение RGB control.
};

enum RotateAngle 
{ 
    ANGLE_0 = 0x00, // портретное расположение 
    ANGLE_90 = 0x60, // вправо на 90 
    ANGLE_180 = 0xC0, // вправо на 180 
    ANGLE_270 = 0xA0 // вправо на 270
};

class GMT147SPI 
    : public SPIDisplay
{
private:
    RotateAngle _angle; // угол поворота

    void reset(); // сброс контроллера 
    bool init() override; // инициализация

    void write(const uint8_t* bytes, size_t len) override; // запись массива однобайтовых последовательностей указанной длины
    void write(const uint16_t* words, size_t len) override; // запись массива двухбайтовых последовательностей указанной длины
    void write(const Rect &rect, const uint8_t* bytes, size_t len) override; // запись массива двухбайтовых последовательностей заявленной длины

    void fill(Area2D &area, const color_t color) override; // заполнение дисплея цветом фона
    void draw(Area2D &area, const uint16_t* image) override; // вывод изображения (RGB565, без сжатия)
    void draw(Point2D &point, const uint16_t pixel) override; // рисование пикселя

    void setArea(Area2D &area); // определить область вывода

    void command(const Commands &command);  // выполнение комманды

    void setAngle(const RotateAngle &angle); // поворот на угол заменой ширины на высоту
    inline const RotateAngle& angle() { return _angle; } // возвращает угол на который сделан поворот

public:
    GMT147SPI(SPIBus *spi, HeadQueue *queue, Kernel::Clock::duration_u32 delay);

    void operator<<(Text &text); // вывод строки
    void operator<<(const Image &image); // вывод изображения

    void handle(Message *message) override; // метод обработки сообщений от обьекта
};

#endif // GMT147SPI_H