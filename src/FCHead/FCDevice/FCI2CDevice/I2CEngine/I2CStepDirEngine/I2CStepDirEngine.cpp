#include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/I2CStepDirEngine.h"

uint8_t I2CStepDirEngine::crc(uint8_t* data, size_t length) // Расчет CRC
{
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) 
    {
        crc = crc ^ data[i];
        for (uint8_t j = 0; j < 8; j++) 
        {
            if (crc & 0x80) 
            {
                crc = (crc << 1) ^ 0x07;
            } else 
            {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

// void StepDirEngine::start()
// {
//     if(condition() == Errored)
//     {
//         return;
//     }    
    
//     setCondition(Started); 
// }

// void StepDirEngine::stop()
// {
//     if(condition() == Errored)
//     {
//         return;
//     }

//     if(condition() == Started)
//     {
//         setCondition(Stoped);
//     }
// }

// void StepDirEngine::run()
// {
// }
