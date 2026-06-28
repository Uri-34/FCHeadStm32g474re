#ifndef FC_A4988_STEP_DIR_MOTOR_WITH_END_STOP_H
#define FC_A4988_STEP_DIR_MOTOR_WITH_END_STOP_H

#include <rtos.h>
#include <mutex>
#include <string>

#include "mbed.h"
#include "pinmap.h"

#include "FCDriver.h"
#include "FCI2CBus.h"

class FCA4988StepDirMotorWithEndStop
    : public FCDriver 
{
public:
    // sharedStepPin и sharedDirPin - общие для всех экземпляров этого класса
    FCA4988StepDirMotorWithEndStop(FCI2CBus *bus, 
                                   uint8_t pcfMotorAddress, 
                                   uint8_t pcfEndStopAddress, 
                                   uint8_t motorNumber,      // 0..15
                                   PinName sharedStepPin, 
                                   PinName sharedDirPin,
                                   const string& name, 
                                   FCStateObject* parent = nullptr);
    ~FCA4988StepDirMotorWithEndStop();

    inline void setBumpSize(uint8_t bumpSize) { _bumpSize = bumpSize; }
    inline void setMaxSpeed(uint32_t hz)   { _maxSpeedHz = hz; }
    inline void setHomeSpeed(uint32_t hz)  { _homeSpeedHz = hz; }

    void home();
    void up(uint16_t steps);
    void down(uint16_t steps);
    void stop();

    inline bool isUp() const { return _up; }
    bool isMoving() const { return _isMoving; }
    bool isHomed() const { return _isHomed; }

    inline uint16_t getStepsLeft() const { return _stepsLeft; }    
    inline uint8_t getMotorNumber() const { return _motorNumber; }

    // Статический диспетчер для вызова из глобального обработчика прерываний
    static void dispatchIRQ() { 
        if (_activeMotor) {
            _activeMotor->timerIRQHandler();
        }
    }

private:
    FCI2CBus *_bus = nullptr;

    // Общие аппаратные ресурсы (управляются статически)
    static rtos::Mutex _sharedHwMutex;
    static FCA4988StepDirMotorWithEndStop *_activeMotor;

    // static TIM_TypeDef* SHARED_TIM = TIM2;
    static TIM_TypeDef* const SHARED_TIM;
    
    static constexpr uint32_t     SHARED_TIM_CH = 1;
    static constexpr uint32_t     SHARED_TIM_AF = GPIO_AF1_TIM2;
    static constexpr IRQn_Type    SHARED_TIM_IRQN = TIM2_IRQn;

    const PinName _sharedStepPin;
    mbed::DigitalOut _sharedDirPin;

    // Индивидуальные ресурсы мотора (через PCF8575)
    const uint8_t  _pcfMotorAddr;
    const uint8_t  _pcfEndStopAddr;
    const uint8_t  _motorNumber;
    
    const uint8_t  _enableBit;    // = motorNumber (0..15) на pcfMotorAddress
    const uint8_t  _endStopBit;   // = motorNumber (0..15) на pcfEndStopAddress
    
    volatile uint16_t _pcfMotorState;  // Кэш для read-modify-write
    volatile uint8_t  _pollDivCnt = 0; // Делитель опроса концевика

    // Состояние
    volatile uint16_t _stepsLeft   = 0;
    volatile bool     _isMoving    = false;
    volatile bool     _isHoming    = false;
    volatile bool     _isHomed     = false;
    volatile uint8_t  _debounceCnt = 0;

    // Настройки
    uint32_t _maxSpeedHz  = 1000;
    uint32_t _homeSpeedHz = 500;
    uint8_t  _bumpSize    = 1;
    bool     _up          = false;

    // Внутренние методы
    void configureSharedStepGPIO();
    void startSharedTimer(uint32_t freq_hz);
    void disableSharedTimer();
    bool limitSwitchActive();
    void setMotorEnable(bool en);
    bool pcfRead(uint8_t addr, uint16_t& out) const;
    bool pcfWrite(uint8_t addr, uint16_t value) const;
    void timerIRQHandler();
};

#endif // FC_A4988_STEP_DIR_MOTOR_WITH_END_STOP_H