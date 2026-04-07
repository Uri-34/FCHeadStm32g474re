#include "Object/Process/ThreadProcess/Mixer/Mixer.h"
#include "Object/Process/ThreadProcess/Conditioner/Conditioner.h"
#include "Object/Process/ThreadProcess/SPIDisplay/SPIDisplay.h"
#include "Object/Process/ThreadProcess/LedIndicator/LedIndicator.h"
#include "Object/Process/ThreadProcess/LaserScaner/LaserScaner.h"

LaserScanner::LaserScanner(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay) 
    : ThreadProcess(queue, "Сканер", LaserScanerID, osPriorityNormal, delay), 
    _i2c{i2c}
{
    if(is(Ready))
    {
        set(Started);
    }   
}

void LaserScanner::append(uint8_t addr) 
{ 
    static int number;
    _sensors.push_back(new VL53L3CXI2C(_i2c, "", addr)); 
    number++;
}

const VL53L3CXI2CData& LaserScanner::get(uint8_t number)
{
    _data = 0;
    reset(Ready);
    if(_sensors.empty() || !isBounded<int>(0, number, _sensors.size()))
    {
        Err("%s - %d", ErrorNumber, number);
        return _data;
    }

    if(!queue())
    {
        Err("Очередь не существует !!!");
        return _data;
    }

    // ..............................
        
    // считать данные датчик(а/ов) из _list по новеру активн(ого/ых) датчик(а/ов)
    // 

    queue()->send(new Message(this, GMT147SPIID, Show, new LaserScannerData(100)));

    set(Ready);
    return _data;
}

const VL53L3CXI2CData& LaserScanner::get(const Point3D &point)
{
    uint8_t number = 0; // = расчет номера датчика в зависимости от направления движения головки  

    return get(number);
}    

// void LaserScanner::doWork(Message *message)
// {
//     // обрабатываем сообщение поступившее миксеру от внешнего источника 
//     vector<string> strCmdList = { "Nope", "Start", "Stop", "Show", "Hide", "ChangeColor" };
//     Msg("%s получил от %s команду %s", 
//          name().c_str(), 
//                        message->source()->name().c_str(), 
//                                   strCmdList[message->command()].c_str());
    
//     switch(message->source()->id())                                  
//     {
//         case ConditionerID:
//             ConditionerData data = static_cast<ConditionerMessage*>(message)->data();

//             // обработка

//             delete static_cast<ConditionerMessage*>(message);
//         break;

//         case IndicatorID:
//             IndicatorData data = static_cast<IndicatorMessage*>(message)->data();

//             // обработка

//             delete static_cast<IndicatorMessage*>(message);
//         break;

//         case MixerID:
//             MixerData data = static_cast<MixerMessage*>(message)->data();

//             // обработка

//             delete static_cast<MixerMessage*>(message);
//         break;
//         default:;
//     }    
// }

void LaserScanner::generate()
{
    // static uint32_t cnt = 0;

    sendMessage(GMT147SPIID, Show, new LaserScannerData(static_cast<uint32_t>(random(0, 255))));
        // Msg("сканер -> дисплей: data=%d : отправленно - %d", 
        //                             static_cast<int>(data.value()),
        //                                                 cnt);

    // cnt++;  
}
