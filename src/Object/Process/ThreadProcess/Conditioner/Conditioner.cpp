#include "Object/Process/ThreadProcess/Conditioner/Conditioner.h"

Conditioner::Conditioner(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay) 
        : ThreadProcess{queue, "Кондиционер", ConditionerID, osPriorityNormal, delay},
          _lm73{new I2CLM73(i2c, "датчик температуры", LM73I2CAddress)},
          _fan{new I2CFan(i2c, "вентилятор", MainFanI2CAddress)}
{
    if(is(Ready) && _lm73->is(Ready) && _fan->is(Ready))
    {
       set(Started); 
    }
}        

Conditioner::~Conditioner() 
{ 
    delete _fan;
    delete _lm73;
}    

// void Conditioner::handler(Message *message)
// {
// }

void Conditioner::generate()
{
    int temp = random(0, 100);
    int speed = random(0, 100);

    // static uint32_t cnt = 0;

    sendMessage(GMT147SPIID, Show, new ConditionerData(temp, speed));

    // cnt++;                                                                                             
}
