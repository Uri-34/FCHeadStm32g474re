#include "FCI2CBus.h"

const string FCI2CBus::scan() 
{ 
    string result = "\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n";
    char buf[6];
    for (int row = 0; row < 0x80; row += 0x10) 
    {
        sprintf(buf, "%02x: ", row);
        result += buf;

        for (int col = 0; col < 16; col++) 
        {
            int addr = row + col;

            if (addr < 0x03 || addr > 0x77) // Исключаем зарезервированные адреса
            {
                sprintf(buf, "   ");
            } 
            else 
            {
                int ret = write(addr << 1, nullptr, 0); // Проверяем наличие устройства
                if (ret == 0) 
                {
                    sprintf(buf, "%02x ", addr);
                } 
                else 
                {
                    sprintf(buf, "-- ");
                }
            }
            result += buf;
        }
        result += "\n";
    }
    return result;
}