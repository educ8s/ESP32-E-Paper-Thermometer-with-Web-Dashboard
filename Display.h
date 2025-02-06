#pragma once

#include <Inkplate.h>
#include "thermometer.h"

class Display {
public:
    Display();
    void init();
    void clear();
    void drawFace();
    void drawTemperature(float temperature);
    bool needsRedraw(float currentTemp, float previousTemp);

private:
    Inkplate display;
    const uint8_t* face;
    int faceWidth;  
    int faceHeight;
};
