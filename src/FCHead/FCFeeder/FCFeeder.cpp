#include "FCFeeder.h"
#include <algorithm>

FCFeeders::FCFeeders(FCI2CBus &i2c, const std::string name, FCStateObject *parent)
    : FCProcess(name, FeedersManager, osPriorityNormal, 1000ms),
      _currentFeeder(0),
      _activeFeeder(UINT8_MAX)
{
    // ⛔ home() НЕ вызываем здесь! RTOS ещё может быть не готов.
    // Инициализация выполняется в initialize() после старта шедулера.

    // ось z
    _feedersVector.push_back(new FCFeeder(i2c)); // 0 

    // фидеры по номерам
    _feedersVector.push_back(new FCFeeder(i2c)); // 1
    _feedersVector.push_back(new FCFeeder(i2c)); // 2
    _feedersVector.push_back(new FCFeeder(i2c)); // 3
    _feedersVector.push_back(new FCFeeder(i2c)); // 4
    _feedersVector.push_back(new FCFeeder(i2c)); // 5
    _feedersVector.push_back(new FCFeeder(i2c)); // 6
    _feedersVector.push_back(new FCFeeder(i2c)); // 7
    // _feeders.init();
}

FCFeeders::~FCFeeders() 
{
    std::lock_guard<rtos::Mutex> lock(_mutex);
    _feedersVector.clear();
}

// void FCFeeders::operator << (FCA4988StepDirMotorWithEndStop* feeder) 
// {
//     if(feeder) 
//     {
//         std::lock_guard<rtos::Mutex> lock(_mutex);
//         _feeders.push_back(feeder);
//     }    
// }

uint8_t FCFeeders::numberActiveFeeder() const 
{
    std::lock_guard<rtos::Mutex> lock(_mutex);
    return _activeFeeder;
}

FCFeeder* FCFeeders::ptrToActiveFeeder() const 
{
    std::lock_guard<rtos::Mutex> lock(_mutex);
    if(_activeFeeder == UINT8_MAX || _activeFeeder >= _feedersVector.size()) 
    {
        return nullptr;
    }
    
    return _feedersVector[_activeFeeder];
}

bool FCFeeders::init() 
{
    // printf("[FM] Initializing feeders... Homing all.\r\n");
    std::lock_guard<rtos::Mutex> lock(_mutex);
    
    if(_feedersVector.empty()) 
    {
        // printf("[FM] WARNING: No feeders registered!\r\n");
        return false;
    }

    for(size_t count = 0; count < _feedersVector.size(); ++count) 
    {
        // printf("[FM] Homing feeder %zu...\r\n", i);
        _feedersVector[count]->home();
        if(!waitForMoveComplete(_feedersVector[count], MOVE_TIMEOUT_MS)) 
        {
            // printf("[FM] ERROR: Homing failed for feeder %zu\r\n", i);
            _activeFeeder = UINT8_MAX;
            return false;
        }
    }
    
    _activeFeeder = UINT8_MAX; // Все в парковке (Home)
    // printf("[FM] All feeders homed and parked successfully.\r\n");
    return true;
}

bool FCFeeders::switchTo(uint8_t targetIdx) 
{
    std::lock_guard<rtos::Mutex> lock(_mutex);

    if(!FCRange<uint8_t>(0, _feedersVector.size()).inrange(targetIdx)) 
    {
        return false; // ошибка в номере фидера
    }

    if(_activeFeeder == targetIdx) 
    {
        return true; // уже активен
    }

    FCFeeder* targetFeeder = _feedersVector[targetIdx];
    FCFeeder* currentFeeder = (_activeFeeder != UINT8_MAX) ? _feedersVector[_activeFeeder] : nullptr;

    // 1. Убираем текущий фидер из рабочей зоны (возврат в Home)
    if(currentFeeder) 
    {
        // printf("[FM] Stowing current feeder %u to Home...\r\n", _activeFeederIdx);
        currentFeeder->home();
        if(!waitForMoveComplete(currentFeeder, MOVE_TIMEOUT_MS)) 
        {
            // printf("[FM] ERROR: Failed to stow feeder %u\r\n", _activeFeederIdx);
            return false;
        }
    }

    // 2. Вводим целевой фидер в рабочую зону
    // printf("[FM] Engaging feeder %u to Work position...\r\n", targetIdx);
    // Предполагаем, что UP = движение от Home в рабочую зону. 
    // Если ваша механика инвертирована, замените на down()
    targetFeeder->up(WORK_POSITION_STEPS);
    if(!waitForMoveComplete(targetFeeder, MOVE_TIMEOUT_MS)) 
    {
        // printf("[FM] ERROR: Failed to engage feeder %u\r\n", targetIdx);
        return false;
    }

    _activeFeeder = targetIdx;
    _currentFeeder = targetIdx;
    // printf("[FM] Feeder %u is now active and in working zone.\r\n", targetIdx);
    return true;
}

// Вспомогательные методы
bool FCFeeders::moveToHome(FCFeeder* feeder) 
{
    if(!feeder) 
    {
        return false;
    }

    feeder->home();
    return waitForMoveComplete(feeder, MOVE_TIMEOUT_MS);
}

bool FCFeeders::moveToWork(FCFeeder* feeder) 
{
    if(!feeder)
    {
        return false;
    }

    feeder->up(WORK_POSITION_STEPS);
    return waitForMoveComplete(feeder, MOVE_TIMEOUT_MS);
}

bool FCFeeders::waitForMoveComplete(FCFeeder* feeder, uint32_t timeout_ms) 
{
    if(!feeder) 
    {
        return false;
    }

    auto start = std::chrono::steady_clock::now();
    
    while(feeder->isMoving()) 
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        
        if(elapsed_ms >= timeout_ms) 
        {
            feeder->stop(); // Аварийная остановка

            return false;
        }
        
        ThisThread::sleep_for(5ms);
    }
    return true;
}

void FCFeeders::exec() 
{
    // Фоновая диагностика или обработка очереди команд
    // Если FCProcess запускает этот метод в отдельном потоке
    // while(true) 
    // {
    //     // Пример: мониторинг ошибок драйверов, логирование состояния и т.д.
    //     ThisThread::sleep_for(1s);
    // }
}