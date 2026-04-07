#ifndef PWM_FAN_H
#define PWM_FAN_H

#include "Head/Device/PwmDevice/PwmDevice.h"
#include "Pins.h"

/*!
	\brief Класс для организации работы обьекта PWM вентилятора 
*/
class PwmFan : public PwmDevice
{
private:
    //! константа - минимальная скорость
    const float MIN_SPEED = 0.0f;

    //! константа - максимальная скорость
    const float MAX_SPEED = 1.0f;

    //! скорость вращения вентилятора  
    float _speed = 0.0;

    //! константа - период
    const float PERIOD = 1.0f / 20000.0f;

    //! ШИМ порт
    PwmOut *_pwm = nullptr;

public:
    PwmFan(const string &name, PinName &pin);
   ~PwmFan();
};

#endif // PWM_FAN_H