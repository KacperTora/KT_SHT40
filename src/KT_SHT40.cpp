/*
 * KT_SHT40 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#include "KT_SHT40.h"

KT_SHT40::KT_SHT40(TwoWire *wire){
    _wire = wire;
    _debugPort = nullptr;
    _precision = KT_SHT40_PRECISION_HIGH;
    _delay = 10;
    _temp = 0.0f;
    _hum = 0.0f;
    _measureTime = 0;
    _serialNumber = 0;
    _heaterMode = KT_SHT40_HEATER_HIGH_LONG;
    _delayHeater = 1100;
    _addr = 0x44;
}

/*!
 *    @brief  Sets up the hardware, initializes I2C and sets high precision
 *    @param  wire
 *            The Wire object (I2C)
 *    @return True if success
 */
bool KT_SHT40::begin(TwoWire *wire, kt_sht40_addr_enum addr){

  if (wire != nullptr) {
    _wire = wire;
  }

  if(!isConnected()){

    if (_debugPort != nullptr) _debugPort->println("SHT40 is not connected!");
    return false;
  }

  setPrecision(KT_SHT40_PRECISION_HIGH);

  _measureTime = millis() - KT_SHT40_DELAY;

  _addr = addr;

  return true;
}

bool KT_SHT40::isConnected(){

  _wire->beginTransmission(_addr);
  return ( _wire->endTransmission() == 0);
}

uint8_t KT_SHT40::_calculateCRC(uint8_t* data){

    uint8_t crc = 0xff;
    size_t i, j;
    for (i = 0; i < 2; i++) {
        crc ^= data[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x80) != 0){
                crc = (uint8_t)((crc << 1) ^ 0x31);
            }
            else{
                crc <<= 1;
            }
        }
    }

    return crc;
}

bool KT_SHT40::setPrecision(kt_sht40_precision_enum value){
    _precision = value;
    _setDelay();
    return true;
}

bool KT_SHT40::setHeaterMode(kt_sht40_heater_enum mode){
    _heaterMode = mode;
    _setDelayHeater();
    return true;
}

void KT_SHT40::_setDelay(){
    switch(_precision){
        case KT_SHT40_PRECISION_HIGH:
        _delay = 10;
        break;

        case KT_SHT40_PRECISION_MEDIUM:
        _delay = 6;
        break;

        case KT_SHT40_PRECISION_LOW:
        _delay = 3;
        break;

        default:
        _delay = 10;
        break;
    }
}

void KT_SHT40::_setDelayHeater(){
    switch(_heaterMode){
        case KT_SHT40_HEATER_HIGH_LONG:
        case KT_SHT40_HEATER_MEDIUM_LONG:
        case KT_SHT40_HEATER_LOW_LONG:
        _delayHeater = 1100;
        break;
        
        case KT_SHT40_HEATER_HIGH_SHORT:
        case KT_SHT40_HEATER_MEDIUM_SHORT:
        case KT_SHT40_HEATER_LOW_SHORT:
        _delayHeater = 200;
        break;

        default:
        _delayHeater = 1100;
        break;
    }
}

bool KT_SHT40::_readData(uint8_t reg, int delayMode, bool heater){

    if(!heater && millis() - _measureTime < KT_SHT40_DELAY) return false;

    _wire -> beginTransmission(_addr);
    _wire -> write(reg);
    if (_wire->endTransmission() != 0) return false;

    delay(delayMode);

    _wire -> requestFrom((uint8_t)_addr, (uint8_t)6);

    uint8_t data[6];
    if(_wire -> available() >= 6){

        for(int i = 0; i < 6; i++) data[i] = _wire -> read();

        if(_calculateCRC(&data[0]) != data[2]) return false;

        if(_calculateCRC(&data[3]) != data[5])  return false;

        
        if (!heater) {
            uint16_t rawTemp = ((uint16_t)data[0] << 8) | data[1];
            _temp = -45.0f + 175.0f * ((float)rawTemp / 65535.0f);

            uint16_t rawHum = ((uint16_t)data[3] << 8) | data[4];
            _hum = -6.0f + 125.0f * ((float)rawHum / 65535.0f);

            if (_hum < 0.0f)   _hum = 0.0f;
            if (_hum > 100.0f) _hum = 100.0f;

            _measureTime = millis();
        }

        return true;
    }

    return false;
}

bool KT_SHT40::_measure(){
    return _readData(_precision, _delay, false);
}

bool KT_SHT40::setHeater(){
    return _readData(_heaterMode, _delayHeater, true);
}

bool KT_SHT40::_readSerialNumber(){
    _wire -> beginTransmission(_addr);
    _wire -> write(KT_SHT40_SERIAL_NUMBER);
    if (_wire->endTransmission() != 0) return false;

    delay(2);

    _wire -> requestFrom((uint8_t)_addr, (uint8_t)6);

    uint8_t data[6];
    if(_wire -> available() >= 6){

        for(int i = 0; i < 6; i++) data[i] = _wire -> read();

        if(_calculateCRC(&data[0]) != data[2]) return false;

        if(_calculateCRC(&data[3]) != data[5])  return false;

        _serialNumber = ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[3] << 8) | data[4];

        return true;
    }

    return false;
}

bool KT_SHT40::reset(){
    _wire -> beginTransmission(_addr);
    _wire -> write(KT_SHT40_RESET);
    bool status = (_wire->endTransmission() == 0);
    delay(2);
    return status;
}

float KT_SHT40::getTemp(){
    _measure();
    return _temp;
}

float KT_SHT40::getHum(){
    _measure();
    return _hum;
}

void KT_SHT40::changeADDR(kt_sht40_addr_enum addr){
    _addr = addr;
}

uint32_t KT_SHT40::getSerialNumber(){
    if (_serialNumber == 0) _readSerialNumber();
    return _serialNumber;
}