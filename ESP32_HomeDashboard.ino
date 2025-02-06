#include <WiFi.h>
#include <WebServer.h>
#include "index_html.h"
#include "SensorData.h"
#include "Display.h"

const char* ssid = "yourssid";
const char* password = "yourpassword";
const bool metric = true;

Sensor sensor(metric);
WebServer server(80);
SensorData sensorData(48, sensor);
Display epaper;

unsigned long lastTempUpdateTime = 0;
const unsigned long tempUpdateInterval = 300000;  // 5 minutes
float lastTemperature = 0.0;

void setup() {
    Serial.begin(115200);
    sensor.begin();
    epaper.init();
    connectToWiFi();
    setupServer();
}

void loop() {
    server.handleClient();

    static int tempUpdateCount = 0;
    static bool firstRun = true;

    if (millis() - lastTempUpdateTime >= tempUpdateInterval || lastTempUpdateTime == 0) {
        lastTempUpdateTime = millis();

        float currentTemperature = sensor.getTemperatureC();

        if (currentTemperature != lastTemperature) {
            Serial.print("Temperature changed: ");
            Serial.println(currentTemperature);

            if (epaper.needsRedraw(currentTemperature, lastTemperature)) {
                epaper.clear();
                epaper.drawFace();
                epaper.drawTemperature(currentTemperature);
            }

            lastTemperature = currentTemperature;
        }

        if (firstRun || ++tempUpdateCount >= 12) {
            Serial.println(firstRun ? "First run: Saving data to sensorData..." : "Saving hourly data to sensorData...");
            sensorData.update();
            tempUpdateCount = 0;
            firstRun = false;
        }
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setupServer() {
    server.on("/sensor", HTTP_GET, []() {
        String jsonResponse = sensorData.getCurrentDataJson();
        jsonResponse.replace("}", ", \"metric\": " + String(metric) + " }");
        server.send(200, "application/json", jsonResponse);
    });

    server.on("/history", HTTP_GET, []() {
        String jsonResponse = sensorData.getHistoricalDataJson();
        server.send(200, "application/json", jsonResponse);
    });

    server.on("/", HTTP_GET, []() {
        server.send_P(200, "text/html", index_html);
    });

    server.begin();
    Serial.println("HTTP server started");
}