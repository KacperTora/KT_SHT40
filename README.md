# KT_SHT40

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-00979D?logo=arduino)](https://github.com/KacperTora/KT_SHT40)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-F58225?logo=platformio)](https://registry.platformio.org/)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Arduino and PlatformIO library for Sensirion SHT40 temperature and relative humidity sensors. Includes hardware CRC-8 verification, configurable precision, integrated heater control, unique serial number extraction and software reset

Tested and validated on SHT40 Module.

---

## Key Features

* **


---

## Wiring & Pinout

The SHT40 communicates over standard I2C (7-bit address: `0x44`).

| SHT40 Pin | MCU / Connection | Notes |
| :--- | :--- | :--- |
| **SDA** | MCU SDA | Requires 4.7kΩ – 10kΩ pull-up |
| **SCL** | MCU SCL | Requires 4.7kΩ – 10kΩ pull-up|

---

## Basic Configuration Example

```cpp
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
```

---

## API Reference

### Initialization
* `bool begin(TwoWire *wire = nullptr)`: Initializes I2C and disables the internal watchdog timer.


---

## Installation

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps =
    KacperTora/KT_SHT40 @ ^1.0.0
```

### Arduino IDE
1. Open Arduino IDE -> **Sketch** -> **Include Library** -> **Manage Libraries...**
2. Search for `KT_SHT40`
3. Click **Install**

---

## Documentation & References

* 

## License

This library is licensed under the **GNU General Public License v3.0**.