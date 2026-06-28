#ifndef FC_TERMINAL_H
#define FC_TERMINAL_H

#include <string>
#include "mbed.h"
// #include "FCCommon.h" // Раскомментируйте, если нужно

using namespace std;

class FCTerminal 
    : public BufferedSerial 
{
private:
    Mutex _mutex;
    
    FCTerminal(PinName tx, PinName rx, int baud);
    FCTerminal() = delete;
    FCTerminal(FCTerminal const &) = delete;
    ~FCTerminal() = default;
    FCTerminal& operator=(const FCTerminal&) = delete;
    
    // Внутренний метод для вывода
    void out_internal(const char *format, ...);

public:
    static FCTerminal& instance(PinName tx = USBTX, PinName rx = USBRX, int baud = 115200) {
        static FCTerminal _terminal(tx, rx, baud);
        return _terminal;
    }

    // Оператор << для вывода готовых строк
    inline FCTerminal& operator<<(const string &str) { 
        out_internal("%s\r\n", str.c_str()); 
        return *this;
    }
};

// Глобальная функция (имя terminal_out, чтобы совпасть с вызовами в FCHead.cpp)
void out(const string &msg, const char *fmt, ...);

// Макросы
#define Msg(fmt, ...) out("Сообщение", fmt, ##__VA_ARGS__)
#define Err(fmt, ...) out("Ошибка", fmt, ##__VA_ARGS__)

#endif // FC_TERMINAL_H