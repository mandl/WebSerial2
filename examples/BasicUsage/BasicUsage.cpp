// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2026 by Stefan Mandl 

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial2.h>
#include <esp_wifi.h>

// WiFi Credentials
const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

AsyncWebServer server(80);
WebSerial2 logger;

void setup()
{
    Serial.begin(115200);
    WiFi.setSleep(false); // Disable WiFi power save for stable WebSockets
    // Connect to WiFi
    WiFi.begin(ssid, password);

    esp_wifi_set_ps(WIFI_PS_NONE);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());

    // Initialize the WebSerial Library
    logger.begin(&server);
    server.begin();

    logger.println("SYSTEM: WebSerial Logger Started!");
}

void loop()
{
    // Required to clean up WebSocket clients
    logger.loop();

    // Example: Send a log every 1 seconds
    static unsigned long lastLog = 0;
    if (millis() - lastLog > 1000)
    {
        logger.println("System Status: OK - Uptime: " + String(millis() / 1000) + "s");
        lastLog = millis();
    }
}
