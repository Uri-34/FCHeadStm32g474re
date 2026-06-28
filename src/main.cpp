#include "FCHead.h"

// #include <cstdio>  

// #include "FCTerminal.h"

int main() 
{
    // Msg("Mbed OS version: %d.%d.%d\n",
    //        MBED_MAJOR_VERSION,
    //        MBED_MINOR_VERSION,
    //        MBED_PATCH_VERSION);

    // mbed::Channel<int, 4> channel;

    FCHead _head;
           _head.exec();

    return 0;
}