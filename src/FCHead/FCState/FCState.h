#ifndef FC_STATE_H
#define FC_STATE_H

#include <cstdint>

/*
 * Формат кодирования флага (8 бит):
 * [ Бит 7    ] : Признак поля Error (1 = это код ошибки, 0 = обычное 1-битное поле)
 * [ Биты 4-6 ] : ID поля (0=Ready, 1=Started, 2=Error, 3=Paused, 4=Changed)
 * [ Биты 0-3 ] : Для 1-битных полей: 1=Установить, 0=Сбросить.
 *                Для поля Error: конкретный код ошибки (0..15).
 */
enum class FCState 
    : uint8_t 
{
// === Поле Ready (ID=0) ===
    Ready    = 0b0000'0001, // ID=0, Action=1 (Set)
    NotReady = 0b0000'0000, // ID=0, Action=0 (Clear)

    // === Поле Started (ID=1) ===
    Started  = 0b0001'0001, // ID=1, Action=1 (Set)
    Stopped  = 0b0001'0000, // ID=1, Action=0 (Clear)

    // === Поле Error (ID=2, IsError=1). Биты 0-3 = код ошибки ===
    ErrorNone           = 0b1010'0000, // 0  - Ошибок нет
    
    ErrorOpen           = 0b1010'0001, // 1  - Ошибка открытия устройства/файла
    ErrorClose          = 0b1010'0010, // 2  - Ошибка закрытия
    
    ErrorConnection     = 0b1010'0011, // 3  - Ошибка подключения
    ErrorDisconnection  = 0b1010'0100, // 4  - Разрыв соединения
    
    ErrorCreate         = 0b1010'0101, // 5  - Ошибка создания ресурса
    ErrorDestroy        = 0b1010'0110, // 6  - Ошибка уничтожения
    
    ErrorRead           = 0b1010'0111, // 7  - Ошибка чтения
    ErrorWrite          = 0b1010'1000, // 8  - Ошибка записи
    
    ErrorOutOfRange     = 0b1010'1001, // 9  - Выход за диапазон
    
    ErrorConfiguration  = 0b1010'1010, // 10 - Ошибка конфигурации
    
    ErrorSend          = 0b1010'1011, // 11 - Ошибка парсинга
    
    ErrorTimeout        = 0b1010'1100, // 12 - Таймаут операции
    
    ErrorEmergencyStoped= 0b1010'1101, // 13 - Аварийная остановка
    
    ErrorMotion         = 0b1010'1110, // 14 - Ошибка перемещения
    
    ErrorAny            = 0b1010'1111, // 15 - Служебный: "есть ли хоть какая-то ошибка?"

    // === Поле Paused (ID=3) ===
    Paused  = 0b0011'0001, // ID=3, Action=1 (Set)
    Resumed = 0b0011'0000, // ID=3, Action=0 (Clear)

    // === Поле Changed (ID=4) ===
    Changed    = 0b0100'0001, // ID=4, Action=1 (Set)
    NotChanged = 0b0100'0000  // ID=4, Action=0 (Clear)
};

class FCCondition 
{
private:
    uint8_t _raw;

    // Магическая константа для мгновенного получения маски по ID поля
    // ID 0 -> 0x01, ID 1 -> 0x02, ID 2 -> 0x3C, ID 3 -> 0x40, ID 4 -> 0x80
    static constexpr uint8_t getMask(uint8_t field_id) { return (0x80403C0201ULL >> (field_id * 8)) & 0xFF; }

public:
    FCCondition() 
        : _raw(0) 
    {}

    explicit FCCondition(uint8_t raw_value) 
        : _raw(raw_value) 
    {}

    void set(FCState state) 
    {
        uint8_t f = static_cast<uint8_t>(state);
        
        bool is_error = (f & 0x80) != 0;
        uint8_t field_id = (f >> 4) & 0x07;
        uint8_t action_or_code = f & 0x0F;

        uint8_t mask = getMask(field_id);
        uint8_t val_to_set;

        if(is_error) 
        {
            // Для ошибки: action_or_code — это сам код (0-15). Сдвигаем его на 2 бита влево
            val_to_set = (action_or_code & 0x0F) << 2;
        } 
        else 
        {
            // Для 1-битных полей: action_or_code — это 1 (установить) или 0 (сбросить)
            val_to_set = action_or_code ? mask : 0;
        }

        // Атомарная операция: очистка и установка
        _raw = (_raw & ~mask) | val_to_set;
    }

    // === ЕДИНЫЙ МЕТОД ПРОВЕРКИ ===
    bool is(FCState state) const 
    {
        uint8_t f = static_cast<uint8_t>(state);
        bool is_error = (f & 0x80) != 0;
        uint8_t field_id = (f >> 4) & 0x07;
        uint8_t mask = getMask(field_id);

        if(is_error) 
        {
            // Проверяем, совпадает ли текущий код ошибки с запрошенным
            uint8_t current_error = (_raw & mask) >> 2;
            uint8_t requested_code = f & 0x0F;
            return current_error == requested_code;
        } 
        else 
        {
            // Для 1-битных полей: просто проверяем, установлен ли бит
            return (_raw & mask) != 0;
        }
    }

    // === ЕДИНЫЙ МЕТОД ПОЛУЧЕНИЯ ===
    uint8_t get(FCState state) const 
    {
        uint8_t f = static_cast<uint8_t>(state);
        bool is_error = (f & 0x80) != 0;
        uint8_t field_id = (f >> 4) & 0x07;
        uint8_t mask = getMask(field_id);

        if(is_error) 
        {
            // Возвращает текущий код ошибки (0-15)
            return (_raw & mask) >> 2;
        } 
        else 
        {
            // Возвращает 1 или 0 для обычных флагов
            return (_raw & mask) ? 1 : 0;
        }
    }

    bool condition(FCState state) { return get(state) ? true : false; } 

    uint8_t getRaw() { return _raw; }
    void setRaw(uint8_t value) { _raw = value; }
    void clear() { _raw = 0; }
};

static_assert(sizeof(FCCondition) == 1, "Размер класса FCState должен быть ровно 1 байт!");

#endif // FC_STATE_H
