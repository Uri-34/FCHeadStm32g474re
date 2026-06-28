#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include "Object/Process/ThreadProcess/ThreadProcess.h"
#include "Head/Indicators/WS2811FIndicator/WS2811FIndicator.h"

class IndicatorData
{
private:
    RGB _rgb;
    uint8_t _numLeds;

public:
    IndicatorData()
        : _rgb{0},
          _numLeds{0}
    {} 

    IndicatorData(RGB &rgb, uint8_t numLeds = 1)
        : _rgb{rgb},
          _numLeds{numLeds}
    {} 

    IndicatorData(IndicatorData &data)
        : _rgb{data._rgb},
          _numLeds{data._numLeds}
    {} 

    inline bool operator==(IndicatorData &data)
    {
        return ((data._rgb == _rgb) && (data._numLeds == _numLeds));
    }

    inline bool operator!=(IndicatorData &data)
    {
        return ((data._rgb != _rgb) || (data._numLeds != _numLeds));
    }

    inline void operator=(IndicatorData &data)
    {
        _rgb = data._rgb;
        _numLeds = data._numLeds;
    }

    inline RGB& rgb() { return _rgb; }
    
    inline uint8_t leds() { return _numLeds; }
};

// class IndicatorMessage 
//     : public Message
// {
// private:
//     IndicatorData _data;

// public:
//     IndicatorMessage(ThreadProcess *source, ProcessID dest, Command command, IndicatorData data) 
//         : Message(source, dest, command),
//           _data{data}  
//     {} 
    
//     inline IndicatorData& data() { return _data; }
// };

class LedIndicator 
    : public ThreadProcess
{
private:
    WS2811FIndicator *_indicator = nullptr;

    void setData(IndicatorData &data);
    
    inline void show() { _indicator->show(); }

public:
    const string ErrorNumberSensor = "ошибочный номер датчика: ";

    LedIndicator(HeadQueue *queue, Kernel::Clock::duration_u32 delay); 

    // метод обработки сообщений приходящих для данного обьекта
    void handle(Message* message) override;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // LED_INDICATOR_H