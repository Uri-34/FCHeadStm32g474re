#include "Head/Device/I2CDevice/I2CEngine/I2CStepDirEngine/TMC2209I2CEngine/TMC2209I2CEngine.h"

uint32_t TMC2209I2CEngine::send(uint8_t address, uint32_t data) 
{
    uint8_t sendBuffer[8];
    uint8_t ansverBuffer[8];

    // Формирование пакета команды
    sendBuffer[0] = 0x05; // Sync byte
    // sendBuffer[1] = address(); // Адрес регистра ???
    // sendBuffer[2] = registerAddress; // Команда (read/write) ???
    sendBuffer[3] = (data >> 24) & 0xFF;
    sendBuffer[4] = (data >> 16) & 0xFF;
    sendBuffer[5] = (data >> 8) & 0xFF;
    sendBuffer[6] = data & 0xFF;
    sendBuffer[7] = crc(sendBuffer, 7); // CRC

    // Отправка команды

    // Чтение ответа

    // Проверка CRC ответа
    if (crc(ansverBuffer, 7) != ansverBuffer[7]) {
        return 0xFFFFFFFF; // Ошибка CRC
    }

    // Возвращаем данные из ответа
    return (ansverBuffer[3] << 24) | (ansverBuffer[4] << 16) | (ansverBuffer[5] << 8) | ansverBuffer[6];
}
