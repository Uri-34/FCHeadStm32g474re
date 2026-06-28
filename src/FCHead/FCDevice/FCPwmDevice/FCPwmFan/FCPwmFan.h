#ifndef FC_PWM_FAN_H
#define FC_PWM_FAN_H

#include "FCPwmDevice.h"
#include "Pins.h"

/*!
	\brief Класс для организации работы обьекта PWM вентилятор 
*/
class FCPwmFan 
    : public FCPwmDevice
{
private:
    //! константа - минимальная частота
    const uint MIN_FREQ = 0;
    //! константа - максимальная частота
    const uint MAX_FREQ = 25000;

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

protected:
    virtual bool init() override
    {
        return true;
    }

public:
    FCPwmFan(const string &name, const PinName pin, uint freq = 25000);
   ~FCPwmFan();

   inline void setSpeed(uint speed) { _speed = bound<float>(MIN_SPEED, speed, MAX_SPEED);; }
   inline float speed() { return _speed; }

   inline void setFrequency(uint frequency) { setFrequency(bound<uint>(MIN_FREQ, frequency, MAX_FREQ)); }

   // сервисные методы для удобства
   // включение на предыдущей скорости
   inline void on() { _pwm->write(_speed); }
   // выключение
   inline void off() { _pwm->write(0.0f); } 
};

#endif // FC_PWM_FAN_H
