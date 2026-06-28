#ifndef LASER_SCANER_H
#define LASER_SCANER_H

#include <vector>

#include "Common.h"
#include "HeadQueue.h"
#include "VL53L3CXI2C.h"

class LaserScannerData
{
private:    
    uint32_t _value; // для теста в дальнейшем описать более подробные данные получаемые от вектора датчиков

public:    
    LaserScannerData(uint32_t value) 
        : _value{value}
    {}
    
    uint32_t value() { return _value; }
};

// class LaserScannerMessage 
//     : public Message
// {
// private:    
//     LaserScannerData _data;

// public:    
//     LaserScannerMessage(ThreadProcess *source, ProcessID dest, Command command, LaserScannerData data) 
//         : Message(source, dest, command),
//           _data{data}  
//     {};
    
//     inline LaserScannerData& data() { return _data;}
// };

class LaserScanner 
    : public FCThreadProcess
{
private:
    FCI2CBus *_i2c = nullptr; // указатель на внешнюю i2c шину 
    vector<VL53L3CXI2C*> _sensors; // список датчиков
    VL53L3CXI2CData _data = {0}; // данные для датчика
    uint8_t _currentSensor = 0; // новер активного датчика

    bool init();

    const VL53L3CXI2CData& get(uint8_t number);
    const VL53L3CXI2CData& get(const Point3D &point);

public:
    const string ErrorNumber= "ошибочный номер датчика: ";

    LaserScanner(I2CBus *i2c, HeadQueue *queue, Kernel::Clock::duration_u32 delay); 

    void append(uint8_t addr);

    // метод обработки сообщений приходящих для данного обьекта
    // void handler(Message *message) override;
    // метод отправки сообщений от обьекта по времени
    void generate() override;
};

#endif // LASER_SCANER_H