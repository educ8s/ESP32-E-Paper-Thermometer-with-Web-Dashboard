#include "Display.h"

Display::Display()
    : face(thermometer), faceWidth(212), faceHeight(104), display() {}

void Display::init() {
    display.begin();
    display.setRotation(3);
    display.clearDisplay();
}

void Display::clear() {
    display.clearDisplay();
}

void Display::drawFace() {
    display.drawImage(face, 0, 0, faceWidth, faceHeight);
}

bool Display::needsRedraw(float currentTemp, float previousTemp) {
    int currentRectWidth = map(currentTemp, -13, 42, 0, 170);
    int previousRectWidth = map(previousTemp, -13, 42, 0, 170);
    return currentRectWidth != previousRectWidth;
}

void Display::drawTemperature(float temperature) {
    int rectWidth = map(temperature, -13, 42, 0, 170); 
    int x = 38;
    int y = 47;
    display.fillRect(x, y, rectWidth, 9, INKPLATE2_RED); 
    display.display();
}