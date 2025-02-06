#include "sensor.h"

Sensor::Sensor(bool useMetric) : metric(useMetric) {}

bool Sensor::begin() {
    return bme.begin(0x76);
}

float Sensor::getTemperature() {
    float tempCelsius = bme.readTemperature();
    return metric ? tempCelsius : (tempCelsius * 1.8f + 32.0f);
}

float Sensor::getTemperatureC() {
    return bme.readTemperature();
}

float Sensor::getHumidity() {
    return bme.readHumidity();
}

float Sensor::getPressure() {
    return bme.readPressure() / 100.0F; // Convert Pa to hPa
}