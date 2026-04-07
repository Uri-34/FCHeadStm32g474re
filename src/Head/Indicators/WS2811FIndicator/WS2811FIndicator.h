#ifndef WS2811F_INDICATOR_H
#define WS2811F_INDICATOR_H

#include <vector>

#include "Head/Indicators/neopixel.h"

#include "PinNames.h"
#include "Pins.h"

#include "Color/RGB/RGB.h"

class WS2811FIndicator : public NeoPixel, public RGB
{
public:
    WS2811FIndicator(PinName pin = MainIndicatorPIN, const int ledNumber = 1) : NeoPixel(pin, ledNumber) 
    {};
};

#endif // WS2811F_INDICATOR_H