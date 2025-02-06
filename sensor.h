#pragma once

#include <Adafruit_BME280.h>

class Sensor {
public:
    Sensor(bool useMetric = true); // Constructor accepts metric flag
    bool begin();
    float getTemperature();
    float getTemperatureC();
    float getHumidity();
    float getPressure();
    
private:
    Adafruit_BME280 bme;
    bool metric;
};
