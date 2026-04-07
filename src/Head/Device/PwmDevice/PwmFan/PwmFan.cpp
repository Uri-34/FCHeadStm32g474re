#include "Head/Device/PwmDevice/PwmFan/PwmFan.h"

PwmFan::PwmFan(const string &name, PinName &pin) 
    : PwmDevice{name, pin}, 
      _pwm{new PwmOut(pin)} 
{}   

PwmFan::~PwmFan()
{
    if(_pwm)
    {
        delete _pwm;
    }
}
