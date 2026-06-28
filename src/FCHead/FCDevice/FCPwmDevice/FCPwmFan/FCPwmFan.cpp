#include "FCPwmFan.h"

FCPwmFan::FCPwmFan(const string &name, const PinName pin, uint frequency) 
    : FCPwmDevice{name, pin},
      _pwm{new PwmOut(pin)} 
{
    setFrequency(frequency);
}   

FCPwmFan::~FCPwmFan()
{
    if(_pwm)
    {
        delete _pwm;
    }
}
