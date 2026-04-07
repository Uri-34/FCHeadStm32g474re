#include "Object/Process/ThreadProcess/Mixer/Mixer.h"
#include "Object/Process/ThreadProcess/Conditioner/Conditioner.h"
#include "Object/Process/ThreadProcess/SPIDisplay/SPIDisplay.h"
#include "Object/Process/ThreadProcess/LedIndicator/LedIndicator.h"
#include "Object/Process/ThreadProcess/LaserScaner/LaserScaner.h"

Mixer::Mixer(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay) 
    : ThreadProcess{queue, "миксер", MixerID, osPriorityNormal, delay},
      _i2c{i2c},
      _ppCyan{new I2CPiezoPump(i2c, "c", 0x00)},
      _ppMagenta{new I2CPiezoPump(i2c, "m", 0x00)},
      _ppYellow{new I2CPiezoPump(i2c, "y", 0x00)}, 
      _ppBlack{new I2CPiezoPump(i2c, "b", 0x00)},
      _ppWhite{new PerelstaticPump(i2c)},
      _blender{new Blender(i2c)}
{
    if(is(Ready))
    {
        set(Started);
    }
}

Mixer::~Mixer()
{
    delete _blender;
    delete _ppWhite;  
    delete _ppBlack;
    delete _ppYellow; 
    delete _ppMagenta;
    delete _ppCyan;
}   

void Mixer::handle(Message* message)
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

    if(message->source()->match(IndicatorID))
    {
        IndicatorData *data = static_cast<IndicatorData*>(static_cast<Message*>(message)->data());

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

void Mixer::generate()
{
    static CMYK cmyk;
    CMYK _cmyk{random(0, 255), 
               random(0, 255), 
               random(0, 255), 
               random(0, 255)};

    if(cmyk != _cmyk)
    {
        cmyk = _cmyk;
        
        // static uint32_t cnt = 0;
        // Msg("миксер -> дисплей: c=%d, m=%d, y=%d, k=%d : оправленно - %d",
        //                           static_cast<int>(cmyk.c()), 
        //                                 static_cast<int>(cmyk.m()), 
        //                                       static_cast<int>(cmyk.y()), 
        //                                             static_cast<int>(cmyk.k()),
        //                                                               cnt
        // );

        sendMessage(GMT147SPIID, Show, new MixerData(cmyk));

        // cnt++;
    }    
}
