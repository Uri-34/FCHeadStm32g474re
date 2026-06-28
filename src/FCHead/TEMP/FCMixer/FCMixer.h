#ifndef MIXER_H
#define MIXER_H

#include "mbed.h"

#include "FCThreadProcess.h"
// #include "FCI2CPiezoPump.h"
// #include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/TMC2209I2CEngine/PerelstaticPump/PerelstaticPump.h"
// #include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/TMC2209I2CEngine/Blender/Blender.h"

#include "RGB.h"
#include "CMYK.h"

// данные передаваемые миксером
class MixerData
{
private:
    CMYK _cmyk;

public:
    MixerData(CMYK cmyk) 
        : _cmyk{cmyk}
    {}    

    CMYK& cmyk() { return _cmyk; }
};

// // сообщение миксера
// class MixerMessage 
//     : public Message
// {
// private:
//     MixerData _data; // данные миксера 

// public:
//     MixerMessage(ThreadProcess *source, ProcessID dest, Command command, MixerData data)
//         : Message(source, dest, command),
//           _data{data}
//     {}

//     MixerData& data() { return _data; } 
// };

class Mixer 
    : public FCThreadProcess
{
private:
    // cmyk палитра
    // CMYK _cmyk;

    // указатель на шину для работы в методах
    I2C *_i2c = nullptr;

    // пьезонасосы
    I2CPiezoPump *_ppCyan = nullptr, // насос голубого красителя
                 *_ppMagenta = nullptr, // насос пурпурного красителя
                 *_ppYellow = nullptr, // насос желтого красителя 
                 *_ppBlack = nullptr; // насос черного красителя
    // насос             
    PerelstaticPump *_ppWhite = nullptr;
    // смеситель
    Blender *_blender = nullptr;

public:
    Mixer(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay); 
   ~Mixer();

    // метод обработки сообщений приходящих для данного обьекта
    void handle(Message *message) override;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // MIXER_H