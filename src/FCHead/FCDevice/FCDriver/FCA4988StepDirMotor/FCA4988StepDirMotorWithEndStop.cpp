#include "FCA4988StepDirMotorWithEndStop.h"
#include <mutex> // Для std::lock_guard

// Инициализация статических членов
rtos::Mutex FCA4988StepDirMotorWithEndStop::_sharedHwMutex;
FCA4988StepDirMotorWithEndStop* FCA4988StepDirMotorWithEndStop::_activeMotor = nullptr;

TIM_TypeDef* const FCA4988StepDirMotorWithEndStop::SHARED_TIM = TIM2;

// Делитель опроса концевика в ISR (I2C опрашивается раз в N шагов)
static constexpr uint8_t LIMIT_POLL_DIVIDER = 20;

FCA4988StepDirMotorWithEndStop::FCA4988StepDirMotorWithEndStop(FCI2CBus *bus,
                                                               uint8_t pcfMotorAddress,
                                                               uint8_t pcfEndStopAddress,
                                                               uint8_t motorNumber,
                                                               PinName sharedStepPin,
                                                               PinName sharedDirPin,
                                                               const string& name,
                                                               FCStateObject* parent)
    : FCDriver(name, StepDirMotor, parent),
        _bus{bus},
        _sharedStepPin(sharedStepPin),
        _sharedDirPin(sharedDirPin),
        _pcfMotorAddr(pcfMotorAddress),
        _pcfEndStopAddr(pcfEndStopAddress),
        _motorNumber(motorNumber),
        _enableBit(motorNumber),      
        _endStopBit(motorNumber),     
        _pcfMotorState(0xFFFF)       
{
    // 1. Настройка общего пина STEP на STM32G474RE (Alternate Function)
    if (_sharedStepPin != NC) {
        configureSharedStepGPIO();
    }

    // 2. Инициализация PCF8575 моторов (чтение во временную переменную для избежания volatile ошибок)
    uint16_t tempState = 0;
    if(pcfRead(_pcfMotorAddr, tempState)) 
    {
        _pcfMotorState = tempState; 
    }
    
    // 3. Инициализация PCF8575 концевиков (установка пина в режим входа с подтяжкой)
    uint16_t esState = 0;
    pcfRead(_pcfEndStopAddr, esState);
    esState |= (1u << _endStopBit); 
    pcfWrite(_pcfEndStopAddr, esState);

    // 4. Гарантируем, что этот мотор выключен при старте
    setMotorEnable(false);
    disableSharedTimer();
}

FCA4988StepDirMotorWithEndStop::~FCA4988StepDirMotorWithEndStop() 
{
    stop();
    setMotorEnable(false);
}

// ============================================================================
// I2C-обёртки для PCF8575 (16 бит, без внутреннего адреса регистра)
// ============================================================================

bool FCA4988StepDirMotorWithEndStop::pcfRead(uint8_t addr, uint16_t& out) const 
{
    char buffer[2] = {0, 0};
    bool ok = const_cast<FCI2CBus*>(_bus)->read(addr, buffer, 2);
    if(ok) 
    {
        out = (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);
    }
    return ok;
}

bool FCA4988StepDirMotorWithEndStop::pcfWrite(uint8_t addr, uint16_t value) const 
{
    char buffer[2] = { (uint8_t)(value & 0xFF), (uint8_t)(value >> 8) };
    return const_cast<FCI2CBus*>(_bus)->write(addr, buffer, 2);
}

// ============================================================================
// Управление индивидуальным ENABLE через PCF8575
// ============================================================================

void FCA4988StepDirMotorWithEndStop::setMotorEnable(bool en) 
{
    if(_enableBit > 15) 
    {
        return;
    }

    // Читаем в локальную переменную
    uint16_t currentState = 0;
    if (!pcfRead(_pcfMotorAddr, currentState)) return;
    
    _pcfMotorState = currentState; // Синхронизируем кэш

    // A4988: ENABLE активен при LOW (0). 
    if (en) {
        _pcfMotorState &= ~(1u << _enableBit);
    } else {
        _pcfMotorState |= (1u << _enableBit);
    }

    pcfWrite(_pcfMotorAddr, _pcfMotorState);
}

// ============================================================================
// Чтение индивидуального концевика через PCF8575
// ============================================================================

bool FCA4988StepDirMotorWithEndStop::limitSwitchActive() 
{
    if(_endStopBit > 15) 
    {
        return false;
    }

    uint16_t state = 0;
    if (!pcfRead(_pcfEndStopAddr, state)) return false;

    // Active Low: концевик замыкает пин на GND → в регистре 0.
    return (state & (1u << _endStopBit)) == 0;
}

// ============================================================================
// Управление общим аппаратным таймером и STEP пином
// ============================================================================

void FCA4988StepDirMotorWithEndStop::configureSharedStepGPIO() 
{
    if(_sharedStepPin != NC) 
    {
        pin_function(_sharedStepPin, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, SHARED_TIM_AF));
    }
}

void FCA4988StepDirMotorWithEndStop::startSharedTimer(uint32_t freq_hz) 
{
    if(freq_hz == 0) 
    {
        return;
    }

    SHARED_TIM->CR1 &= ~TIM_CR1_CEN;

    uint32_t tim_clk = SystemCoreClock;
    uint32_t period_ticks = tim_clk / freq_hz;
    uint16_t psc = 1;
    while ((period_ticks / psc) > 65535) psc++;
    uint16_t arr = (period_ticks / psc) - 1;

    SHARED_TIM->PSC = psc - 1;
    SHARED_TIM->ARR = arr;
    SHARED_TIM->CNT = 0;
    SHARED_TIM->EGR = TIM_EGR_UG;

    uint32_t ch_idx = SHARED_TIM_CH - 1;
    uint32_t shift  = (ch_idx % 2) * 8;

    if(ch_idx < 2) 
    {
        SHARED_TIM->CCMR1 &= ~(0xFFUL << shift);
        SHARED_TIM->CCMR1 |= (TIM_OCMODE_TOGGLE << shift) | (TIM_CCMR1_OC1PE << shift);
    } 
    else 
    {
        SHARED_TIM->CCMR2 &= ~(0xFFUL << shift);
        SHARED_TIM->CCMR2 |= (TIM_OCMODE_TOGGLE << shift) | (TIM_CCMR2_OC3PE << shift);
    }

    uint32_t ccer_shift = ch_idx * 4;
    SHARED_TIM->CCER &= ~(0xFUL << ccer_shift);
    SHARED_TIM->CCER |= (TIM_CCER_CC1E << ccer_shift);

    SHARED_TIM->DIER |= (TIM_DIER_CC1IE << ch_idx);
    SHARED_TIM->SR    = ~(TIM_SR_CC1IF << ch_idx);

    _pollDivCnt = 0;
    SHARED_TIM->CR1 |= TIM_CR1_CEN;
}

void FCA4988StepDirMotorWithEndStop::disableSharedTimer() 
{
    SHARED_TIM->CR1 &= ~TIM_CR1_CEN;
    uint32_t ch_idx = SHARED_TIM_CH - 1;
    SHARED_TIM->DIER &= ~(TIM_DIER_CC1IE << ch_idx);
    uint32_t ccer_shift = ch_idx * 4;
    SHARED_TIM->CCER &= ~(TIM_CCER_CC1E << ccer_shift);
    SHARED_TIM->SR    = ~(TIM_SR_CC1IF << ch_idx);
}

// ISR таймера (вызывается для _activeMotor)
void FCA4988StepDirMotorWithEndStop::timerIRQHandler() 
{
    uint32_t ch_idx = SHARED_TIM_CH - 1;

    if (!(SHARED_TIM->SR & (TIM_SR_CC1IF << ch_idx))) 
    {
        return;
    }

    SHARED_TIM->SR = ~(TIM_SR_CC1IF << ch_idx);

    if(_isHoming) 
    {
        if(++_pollDivCnt >= LIMIT_POLL_DIVIDER) 
        {
            _pollDivCnt = 0;

            if(limitSwitchActive()) 
            {
                if(++_debounceCnt > 10) 
                { 
                    _isHoming = false;
                    _isMoving = false;
                    _isHomed  = true;
                    _stepsLeft = 0;
                    disableSharedTimer();
                    setMotorEnable(false); 
                    _activeMotor = nullptr;
                }
            } 
            else 
            {
                _debounceCnt = 0;
            }
        }
    } 
    else if (_isMoving) 
         {
            if (_stepsLeft > 0)
            { 
                _stepsLeft--;
            }
            
            if (_stepsLeft == 0) 
            {
                _isMoving = false;
                disableSharedTimer();
                setMotorEnable(false); 
                _activeMotor = nullptr;
            }
         }
}

// Публичные команды (с защитой от одновременного доступа к общей шине)
void FCA4988StepDirMotorWithEndStop::home() 
{
    // ✅ УНИВЕРСАЛЬНЫЙ СПОСОБ: работает в Mbed OS 5 и Mbed OS 6
    std::lock_guard<rtos::Mutex> lock(_sharedHwMutex);
    
    if(_isMoving && !_isHoming) 
    {
        return;
    }

    _activeMotor = this; 
    _isHoming    = true;
    _isHomed     = false;
    _isMoving    = true;
    _debounceCnt = 0;
    _pollDivCnt  = 0;

    _sharedDirPin.write(0); 
    setMotorEnable(true);   
    
    _stepsLeft = 0xFFFF;    
    startSharedTimer(_homeSpeedHz);
}

void FCA4988StepDirMotorWithEndStop::up(uint16_t steps) 
{
    // ✅ УНИВЕРСАЛЬНЫЙ СПОСОБ
    std::lock_guard<rtos::Mutex> lock(_sharedHwMutex);
    if (_isMoving) return;

    _activeMotor = this;
    _sharedDirPin.write(1); 
    setMotorEnable(true);
    
    _stepsLeft = steps;
    _isMoving  = true;
    _isHoming  = false;
    startSharedTimer(_maxSpeedHz);
}

void FCA4988StepDirMotorWithEndStop::down(uint16_t steps) 
{
    // ✅ УНИВЕРСАЛЬНЫЙ СПОСОБ
    std::lock_guard<rtos::Mutex> lock(_sharedHwMutex);
    if(_isMoving) 
    {
        return;
    }    

    _activeMotor = this;
    _sharedDirPin.write(0); 
    setMotorEnable(true);
    
    _stepsLeft = steps;
    _isMoving  = true;
    _isHoming  = false;
    startSharedTimer(_maxSpeedHz);
}

void FCA4988StepDirMotorWithEndStop::stop() 
{
    // ✅ УНИВЕРСАЛЬНЫЙ СПОСОБ
    std::lock_guard<rtos::Mutex> lock(_sharedHwMutex);
    if(_isMoving || _isHoming) 
    {
        _isMoving = false;
        _isHoming = false;
        _stepsLeft = 0;
        disableSharedTimer();
        setMotorEnable(false);
        _activeMotor = nullptr;
    }
}
