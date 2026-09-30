/*
 * KT_SHT40 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#pragma once

#include "Arduino.h"
#include "Wire.h"

#define KT_SHT40_DELAY 250

#define KT_SHT40_SERIAL_NUMBER 0x89
#define KT_SHT40_RESET 0x94

enum kt_sht40_precision_enum : uint8_t {
    KT_SHT40_PRECISION_HIGH = 0xFD,
    KT_SHT40_PRECISION_MEDIUM = 0xF6,
    KT_SHT40_PRECISION_LOW = 0xE0
};

enum kt_sht40_heater_enum : uint8_t {
    KT_SHT40_HEATER_HIGH_LONG = 0x39,
    KT_SHT40_HEATER_HIGH_SHORT = 0x32,
    KT_SHT40_HEATER_MEDIUM_LONG = 0x2F,
    KT_SHT40_HEATER_MEDIUM_SHORT = 0x24,
    KT_SHT40_HEATER_LOW_LONG = 0x1E,
    KT_SHT40_HEATER_LOW_SHORT = 0x15
};

enum kt_sht40_addr_enum : uint8_t {
    KT_SHT40_ADDR_0X44 = 0x44,
    KT_SHT40_ADDR_0X45 = 0x45,
    KT_SHT40_ADDR_0X46 = 0x46
};

class KT_SHT40
{
    public:
    explicit KT_SHT40(TwoWire *wire = &Wire);

    bool begin(TwoWire *wire = nullptr, kt_sht40_addr_enum addr = KT_SHT40_ADDR_0X44);
    bool isConnected();
    bool setPrecision(kt_sht40_precision_enum value);
    bool setHeaterMode(kt_sht40_heater_enum mode);
    float getTemp();
    float getHum();
    bool setHeater();
    uint32_t getSerialNumber();
    bool reset();
    void changeADDR(kt_sht40_addr_enum addr);

    void setDebugPort(Stream &debugPort) { _debugPort = &debugPort; }
    void disableDebug() { _debugPort = nullptr; }

    private:

    bool _readData(uint8_t reg, int delayMode, bool heater);
    bool _readSerialNumber();
    bool _measure();
    uint8_t _calculateCRC(uint8_t* data);
    void _setDelay();
    void _setDelayHeater();

    TwoWire* _wire;
    Stream *_debugPort; 
    uint8_t _precision;
    uint8_t _heaterMode;
    float _temp;
    float _hum;
    int _delay;
    int _delayHeater;
    unsigned long _measureTime;
    uint32_t _serialNumber;
    uint8_t _addr;
};