#ifndef FC_WS2811F_INDICATOR_H
#define FC_WS2811F_INDICATOR_H

#include <vector>

#include "neopixel.h"

#include "PinNames.h"
#include "Pins.h"

#include "RGB.h"

class FCWS2811FIndicator 
    : public NeoPixel, public RGB
{
public:
    FCWS2811FIndicator(PinName pin = MainIndicatorPIN, const int ledNumber = 1) 
        : NeoPixel(pin, ledNumber) 
    {};
};

#endif // FC_WS2811F_INDICATOR_H
