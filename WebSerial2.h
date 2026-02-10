
// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2026 by Stefan Mandl

#ifndef WEB_SERIAL2_H
#define WEB_SERIAL2_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"

class WebSerial2
{
public:
    WebSerial2(uint16_t port = 80);
    void begin(AsyncWebServer *server);
    void print(String msg);
    void println(String msg);
    void println(float msg, int decimals = 2U);
    void print(float msg, int decimals = 2U);
    void loop();

private:
    AsyncWebSocket _ws;
    String _logBuffer;
    unsigned long lastBatchTime = 0;
    const int MAX_BUFFER_SIZE = 1024; // Bytes
    const int BATCH_TIMEOUT = 100;    // Milliseconds
    unsigned long _lastBatchTime;
    void _onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                  AwsEventType type, void *arg, uint8_t *data, size_t len);
};

#endif
