#pragma once
#include "sensor.h"
#include <ArduinoJson.h>

class SensorData {
public:
    SensorData(int dataPoints, Sensor& sensor);
    ~SensorData();
    bool begin();
    void update();
    String getCurrentDataJson();
    String getHistoricalDataJson();

private:
    Sensor& sensor;          
    const int DATA_POINTS;   
    int currentIndex;       
    float* temperatureHistory; 
    float* humidityHistory; 
    float* pressureHistory;
};
