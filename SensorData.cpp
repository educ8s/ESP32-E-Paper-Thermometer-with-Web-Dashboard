#include "SensorData.h"
#include <Arduino.h>

SensorData::SensorData(int dataPoints, Sensor& sensorRef)
    : sensor(sensorRef), DATA_POINTS(dataPoints), currentIndex(0) {
    temperatureHistory = new float[DATA_POINTS]();
    humidityHistory = new float[DATA_POINTS]();
    pressureHistory = new float[DATA_POINTS]();
}

SensorData::~SensorData() {
    delete[] temperatureHistory;
    delete[] humidityHistory;
    delete[] pressureHistory;
}

void SensorData::update() {
    float temperature = sensor.getTemperature();
    float humidity = sensor.getHumidity();
    float pressure = sensor.getPressure();

    if (isnan(temperature) || isnan(humidity) || isnan(pressure)) {
        Serial.println("Sensor reading failed. Skipping update.");
        return;
    }

    for (int i = 0; i < DATA_POINTS - 1; i++) {
        temperatureHistory[i] = temperatureHistory[i + 1];
        humidityHistory[i] = humidityHistory[i + 1];
        pressureHistory[i] = pressureHistory[i + 1];
    }

    temperatureHistory[DATA_POINTS - 1] = temperature;
    humidityHistory[DATA_POINTS - 1] = humidity;
    pressureHistory[DATA_POINTS - 1] = pressure;

    Serial.println("Historical data updated:");
    Serial.print("Temperature: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
    Serial.print("Pressure: "); Serial.println(pressure);
}

String SensorData::getCurrentDataJson() {
    float temperature = sensor.getTemperature();
    float humidity = sensor.getHumidity();
    float pressure = sensor.getPressure();

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperature"] = temperature;
    jsonDoc["humidity"] = humidity;
    jsonDoc["pressure"] = pressure;

    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    return jsonResponse;
}

String SensorData::getHistoricalDataJson() {
    StaticJsonDocument<1200> jsonDoc;

    JsonArray tempArray = jsonDoc.createNestedArray("temperature");
    JsonArray humArray = jsonDoc.createNestedArray("humidity");
    JsonArray presArray = jsonDoc.createNestedArray("pressure");

    for (int i = 0; i < DATA_POINTS; i++) {
        tempArray.add(temperatureHistory[i]);
        humArray.add(humidityHistory[i]);
        presArray.add(pressureHistory[i]);
    }

    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    return jsonResponse;
}
