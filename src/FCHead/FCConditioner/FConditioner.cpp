#include "FCConditioner.h"

FCConditioner::FCConditioner(FCI2CBus &i2c, uint8_t address, string name, FCHeadQueue &queue, Kernel::Clock::duration_u32 delay) 
        : FCThreadedProcess{&queue, name, ConditionerID, osPriorityNormal, delay},
          _emc2101{new FCEMC2101(&i2c, name, address)}
{}        

FCConditioner::~FCConditioner() 
{ 
    delete _emc2101;
}    

void FCConditioner::generate()
{
    // заглушка ! заменить реальным чтением и конвертацией температуры и скорости вентилятора
    int temperature = random(0, 100);
    int speed = random(0, 100);

    sendMessage(HeadID, Show, new FCConditionerData(name(), temperature, speed));
}
