#include "Object/Process/ThreadProcess/LedIndicator/LedIndicator.h"
#include "Object/Process/ThreadProcess/Conditioner/Conditioner.h"
#include "Object/Process/ThreadProcess/Mixer/Mixer.h"
#include "Object/Process/ThreadProcess/LaserScaner/LaserScaner.h"

LedIndicator::LedIndicator(HeadQueue *queue, Kernel::Clock::duration_u32 delay) 
    : ThreadProcess(queue, "Индикатор:", IndicatorID, osPriorityNormal, delay),
      _indicator{new WS2811FIndicator}
{
    if(is(Ready))
    {
        set(Started);
        reset(Changed);
    }
}

void LedIndicator::setData(IndicatorData &data)
{
    // записываем данные в led контроллер 
    for(int count = 0; count < min(_indicator->size(), data.leds()); count++)
    {
        _indicator->setColor(data.rgb(), count);
    }
}

void LedIndicator::handle(Message* message)
{
    // обрабатываем сообщение поступившее миксеру от внешнего источника 
    vector<string> strCmdList = { "Nope", "Start", "Stop", "Show", "Hide", "ChangeColor" };
    Msg("%s получил от %s команду %s", 
         name().c_str(), 
                       message->source()->name().c_str(), 
                                  strCmdList[message->command()].c_str());
    
    if(message->source()->match(ConditionerID))
    {
        ConditionerData *data = static_cast<ConditionerData*>(static_cast<Message*>(message)->data());

        // обработка

        delete data;
        delete message;
    }    

    if(message->source()->match(MixerID))
    {
        MixerData *data = static_cast<MixerData*>(static_cast<Message*>(message)->data());

        // обработка

        delete data;
        delete message;
    }

    if(message->source()->match(LaserScanerID))
    {
        LaserScannerData *data = static_cast<LaserScannerData*>(static_cast<Message*>(message)->data());

        // обработка

        delete data;
        delete message;
    }
}

void LedIndicator::generate()
{
    // static uint32_t cnt = 0;
    
    static RGB rgb;
    // если данные индикатора требуют обновления - отправляем данные на дисплей ST7789v3ID
    RGB _rgb(random(0, 255), random(0, 255), random(0, 255));
    if(rgb != _rgb)
    {
        rgb = _rgb;
        sendMessage(GMT147SPIID, Show, new IndicatorData(rgb, 1));
        // Msg("индикатор -> дисплей: r=%d, g=%d, b=%d : отправленно - %d",
        //                             static_cast<int>(rgb.r()), 
        //                                 static_cast<int>(rgb.g()), 
        //                                         static_cast<int>(rgb.b()),
        //                                                             cnt);

        // cnt++;                                                                    
    }
}