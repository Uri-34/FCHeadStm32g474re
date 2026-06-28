#include "FCTerminal.h"
#include <vector>

FCTerminal::FCTerminal(PinName tx, PinName rx, int baud)
    : BufferedSerial(tx, rx, baud)
{
    set_blocking(true); // Гарантируем, что запись дождется отправки байтов
}

void FCTerminal::out_internal(const char *format, ...) {
    ScopedLock<Mutex> lock(_mutex);
    
    va_list args;
    va_start(args, format);
    
    // 1. Измеряем размер через КОПИЮ списка аргументов
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(nullptr, 0, format, args_copy);
    va_end(args_copy);
    
    if (size < 0) {
        va_end(args);
        return;
    }
    
    // 2. Форматируем через ОРИГИНАЛ списка аргументов
    std::vector<char> buffer(size + 1);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args); // Обязательно закрываем оригинал
    
    // 3. Пишем напрямую в UART
    this->write(buffer.data(), size);
}

// ИСПРАВЛЕННАЯ глобальная функция
void out(const string &msg, const char *fmt, ...) 
{
    static uint32_t strNum = 0;
    
    // Блокируем только счетчик, чтобы не блокировать UART надолго
    static Mutex counter_mutex;
    ScopedLock<Mutex> lock(counter_mutex);
    uint32_t current_num = strNum++;
    
    va_list args;
    va_start(args, fmt);
    
    // 1. Измеряем размер через КОПИЮ
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    
    if (size >= 0) {
        // 2. Форматируем через ОРИГИНАЛ
        std::vector<char> buffer(size + 1);
        vsnprintf(buffer.data(), buffer.size(), fmt, args);
        
        // 3. Собираем и выводим
        string final_msg = msg + " " + to_string(current_num) + " : " + string(buffer.data());
        FCTerminal::instance() << final_msg;
    }
    
    va_end(args);
}

// Если функция format() где-то используется, исправляем и её по тому же принципу
string format(const char* fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    
    if(size < 0) 
    {
        va_end(args);
        return "";
    }
    
    vector<char> buffer(size + 1);
    vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);
    
    return string(buffer.data());
}