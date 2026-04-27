#include "PwmFan.h"

FCPwmFan::FCPwmFan(const string &name, PinName &pin) 
    : FCPwmDevice{name, pin}, 
      _pwm{new PwmOut(pin)} 
{}   

FCPwmFan::~FCPwmFan()
{
    if(_pwm)
    {
        delete _pwm;
    }
}
