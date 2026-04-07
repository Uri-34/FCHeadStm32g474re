#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

#include "mbed.h"

#include "Common/Common.h"

class Terminal : public BufferedSerial 
{
private:
    Mutex _mutex;

    Terminal(PinName tx, PinName rx, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    Terminal() = default;
   ~Terminal() = default;
    Terminal(Terminal const &) = delete;
    Terminal& operator=(const Terminal&) = delete;

    void out(const char *format, ...);

public:
    static Terminal& instance(PinName tx = USBTX, PinName rx = USBRX, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) 
    {
        static Terminal _terminal(tx, rx, baud);
        return _terminal; 
    };

    //! метод вывода сообщения об ошибке в терминал
    inline void operator << (const string &str) { out("%s\n", str.c_str()); }
};

// создание форматированной строки 
string format(const char* fmt, ...); 
// форматированный вывод сообщения в консоль
void out(const string &msg, const string &fmt, ...);

// определение для форматированного вывода сообщения в консоль
#define Msg(fmt, ...) out("Сообщение: ", fmt, ##__VA_ARGS__) 
// определение для форматированного вывода ошибки в консоль
#define Err(fmt, ...) out("Ошибка: ", fmt, ##__VA_ARGS__) 

#endif // TERMINAL_H