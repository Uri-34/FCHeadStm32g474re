#include "Terminal/Terminal.h"

Terminal::Terminal(PinName tx, PinName rx, int baud) 
    : BufferedSerial(tx, rx, baud) 
{
    setvbuf(stdout, NULL, _IONBF, 0);
    freopen("/dev/null", "w", stderr);
    mbed_set_unbuffered_stream(fdopen(this, "w"));
}

void Terminal::out(const char* format, ...)
{
    ScopedLock<Mutex> lock(_mutex);
    va_list args;
    va_start(args, format);

    while (*format != '\0') 
    {
        if (*format == '%') 
        {
            format++;
            switch (*format) 
            {
                case 'd': 
                {
                    int d = va_arg(args, int);
                    printf("%d", d);
                    break;
                }
                case 'f': 
                {
                    double f = va_arg(args, double);
                    printf("%f", f);
                    break;
                }
                case 'c': 
                {
                    int c = va_arg(args, int);
                    printf("%c", c);
                    break;
                }
                case 's': 
                {
                    const char* s = va_arg(args, const char*);
                    printf("%s", s);
                    break;
                }
                case '%': 
                {
                    printf("%%");
                    break;
                }
                default:
                    printf("%%%c", *format);
                    break;
            }
        } 
        else 
        {
            printf("%c", *format); // Обычные символы выводим "как есть"
        }
        format++;
    }
    va_end(args);
}

string format(const char* fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
        int size = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    vector<char> buffer(size + 1);
 
    va_start(args, fmt);
        vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);

    return string(buffer.data());
}

void out(const string &msg, const string &fmt, ...)
{
    static uint32_t strNum = 0; 

    va_list args;
    va_start(args, fmt);
        int size = vsnprintf(nullptr, 0, fmt.c_str(), args);
    va_end(args);

    vector<char> buffer(size + 1);

    va_start(args, fmt);
        vsnprintf(buffer.data(), buffer.size(), fmt.c_str(), args);
    va_end(args);

    Terminal::instance() << msg + " " + to_string(strNum) + " : " + buffer.data();
    
    strNum++;
}