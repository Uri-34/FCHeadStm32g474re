#include "FCConditioner.h"

FCConditioner::FCConditioner(FCI2CBus *i2c, FCHeadQueue *queue, Kernel::Clock::duration_u32 delay) 
        : FCThreadProcess{queue, "Кондиционер", ConditionerID, osPriorityNormal, delay},
          _termometr{new FCI2CLM75(i2c, "термометр", LM75I2CAddress)},
          _fan{new FCI2CFan(i2c, "вентилятор", MainFanI2CAddress)}
{}        

FCConditioner::~FCConditioner() 
{ 
    delete _fan;
    delete _termometr;
}    

void FCConditioner::generate()
{
    int temp = random(0, 100);
    int speed = random(0, 100);

    // static uint32_t cnt = 0;

    sendMessage(GMT147SPIID, Show, new FCConditionerData(temp, speed));
}
