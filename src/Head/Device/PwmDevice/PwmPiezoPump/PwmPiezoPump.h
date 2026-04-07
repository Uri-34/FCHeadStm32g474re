#ifndef PWM_PIEZO_PUMP_H
#define PWM_PIEZO_PUMP_H

#include "Head/Device/PwmDevice/PwmDevice.h"

class PwmPiezoPump : public PwmDevice
{
private:
    const string ErrorExists = "насос не найден...";

public:
    PwmPiezoPump(const string &name, const PinName &pin) 
        : PwmDevice(name, pin) 
    {}

    bool ready() override final
    {
        // в дальнейшем доделать и отладить проверку наличия насоса
        return true;
    }

    //! сделать один качек
    void pitch();

    //! сделать number качков
    void pitches(int number)
    {
        while(number > 0)
        {
            pitch();
            number--;
        }
    }
};

#endif // PWM_PIEZO_PUMP_H