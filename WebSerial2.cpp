// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2026 by Stefan Mandl

#include "WebSerial2.h"

WebSerial2::WebSerial2(uint16_t port) : _ws("/webserialws"), _logBuffer(""), _lastBatchTime(0) {}

void WebSerial2::begin(AsyncWebServer *server)
{
    _ws.onEvent([this](AsyncWebSocket *s, AsyncWebSocketClient *c, AwsEventType t, void *a, uint8_t *d, size_t l)
                { this->_onEvent(s, c, t, a, d, l); });
    server->addHandler(&_ws);

    server->on("/webserial", HTTP_GET, [](AsyncWebServerRequest *request)
               {
        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", WEBPAGE_DATA, WEBPAGE_SIZE);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response); });

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->redirect("/webserial"); });
}

void WebSerial2::print(float msg, int decimals)
{
    print(String(msg, decimals));
}

void WebSerial2::println(float msg, int decimals)
{
    println(String(msg, decimals));
}

void WebSerial2::println(String msg)
{
    print(msg + "\n");
}

void WebSerial2::print(String msg)
{
    _logBuffer += msg;

    // Send immediately if buffer is getting large or timeout reached
    if (_logBuffer.length() > MAX_BUFFER_SIZE || (millis() - lastBatchTime > BATCH_TIMEOUT))
    {
        if (_ws.count() > 0 && _ws.availableForWriteAll())
        {
            _ws.textAll(_logBuffer);
            _logBuffer = ""; // Clear buffer
            lastBatchTime = millis();
        }
    }
}

void WebSerial2::loop()
{
    _ws.cleanupClients();
}

void WebSerial2::_onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{

    if (type == WS_EVT_DATA)
    {
        String msg = "";
        for (size_t i = 0; i < len; i++)
            msg += (char)data[i];

        // Handle Ping for Latency Stats
        if (msg == "__ping__")
        {
            client->text("__pong__");
        }
        // Handle actual user commands
        else
        {
            Serial.printf("Command Received: %s\n", msg.c_str());
            // Process your commands here...
        }
    }
}
