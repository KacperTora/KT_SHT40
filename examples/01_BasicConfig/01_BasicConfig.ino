/*
 * KT_SHT40 Arduino Library
 * Copyright (C) 2026 Kacper Tora
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 */

#include <Arduino.h>
#include <Wire.h>
#include <KT_SHT40.h>

KT_SHT40 sht;

void setup(){
    Serial.begin(115200);
    delay(1000);

    Wire.begin();

    sht.setDebugPort(Serial);

    while (!sht.begin(&Wire)) {
        Serial.println("Couldnt find SHT40.");
        delay(2000);
    }
    Serial.println("SHT40 initialized successfully.");
    Serial.print("Serial Number: 0x");
    Serial.println(sht.getSerialNumber(), HEX);

    sht.setPrecision(KT_SHT40_PRECISION_HIGH);
}

void loop(){

    Serial.print("Temperature: ");
    Serial.print(sht.getTemp(), 2);
    Serial.print(" °C | Humidity: ");
    Serial.print(sht.getHum(), 2);
    Serial.println(" %");

    delay(2000);
}