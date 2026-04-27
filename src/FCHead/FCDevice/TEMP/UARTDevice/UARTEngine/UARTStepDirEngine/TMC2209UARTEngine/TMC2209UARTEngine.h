#ifndef TMC2209UART_ENGINE_H
#define TMC2209UART_ENGINE_H

#include <vector>

#include <mbed.h>

#include "Common/Common.h"
#include "Bus/UARTBus/UARTBus.h"
#include "Head/Device/UARTDevice/UARTEngine/UARTStepDirEngine/UARTStepDirEngine.h"

class TMC2209UARTEngine : public UARTStepDirEngine 
{
public:
    // регистры для чтения/записи
    const uint8_t GCONF = 0x00; // глобальная конфигурация драйвера.
    const uint8_t GSTAT = 0x04; // глобальный статус (сброс ошибок).
    const uint8_t IOIN = 0x06; // входные сигналы и состояние выводов.
    const uint8_t IHOLD_IRUN = 0x0A; // настройка тока холостого хода (I_HOLD) и рабочего тока (I_RUN).
    const uint8_t TPOWERDOWN = 0x10; // время до перехода в режим пониженного энергопотребления.
    const uint8_t TPWMTHRS = 0x20; // порог скорости для переключения между StealthChop и SpreadCycle.
    const uint8_t TCOOLTHRS = 0x24; // порог скорости для CoolStep.
    const uint8_t VACTUAL = 0x2D; // установка фактической скорости вращения (ручное управление двигателем).
    const uint8_t SGTHRS = 0x30; // порог чувствительности StallGuard.
    const uint8_t SG_RESULT = 0x31; // результат работы StallGuard (только для чтения).
    const uint8_t COOLCONF = 0x40; // конфигурация CoolStep.
    const uint8_t MSCNT = 0x6C; // текущий микрошаг (только для чтения).
    const uint8_t MSCURACT = 0x6D; // активный ток микродвижений (только для чтения).

    // регистры только для чтения
    const uint8_t IFCNT = 0x01; // cчетчик входящих кадров (только для чтения).
    const uint8_t SLAVECONF = 0x02; // конфигурация подчиненного устройства (не используется напрямую).
    const uint8_t OTP_PROG = 0x03; // программирование однократно программируемой памяти (OTP).
    const uint8_t OTP_READ = 0x05; // чтение OTP (только для чтения).
    const uint8_t FACTORY_CONF = 0x07; // фабричная конфигурация (только для чтения).
    const uint8_t DRV_STATUS = 0x0B; // статус драйвера (ошибки, температура, нагрузка).
    const uint8_t PWM_SCALE = 0x0C; // масштабирование ШИМ (только для чтения).
    const uint8_t LOST_STEPS = 0x0E; // cчетчик потерянных шагов (только для чтения).

    UARTBus *_uart = nullptr;

private:
    uint32_t send(uint8_t registerAddress, uint32_t data); // Отправка команды через UART

public:
    TMC2209UARTEngine(UARTBus *uart, const string &name, const PinName &pin, uint8_t address) 
        : UARTStepDirEngine(name, pin, address),
          _uart{_uart}  
    {}

    inline uint32_t stallGuard() 
    { 
        uint32_t data;
        return send(SGTHRS, data); 
    }
};

#endif // TMC2209UART_ENGINE_H