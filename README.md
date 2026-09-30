# KT_SHT40

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-00979D?logo=arduino)](https://github.com/KacperTora/KT_SHT40)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-F58225?logo=platformio)](https://registry.platformio.org/)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Arduino and PlatformIO library for Sensirion SHT40 temperature and relative humidity sensors. Includes hardware CRC-8 verification, configurable precision, integrated heater control, unique serial number extraction, and software reset.

Tested and validated on SHT40 Module.

---

## Key Features

* **Clean Architecture:** Pure `Wire.h` implementation without third-party base libraries or heavy abstractions.
* **Data Integrity:** Every temperature and humidity packet is mathematically verified using Sensirion's polynomial.
* **Throttling:** Automated 250 ms query rate-limiting prevents bus flooding, redundant transactions, and sensor self-heating.
* **Precision Control:** Support for High, Medium, and Low repeatability modes with dynamic delay adjustments.
* **On-Chip Pulsed Heater:** Full support for integrated heating modes (200 mW, 110 mW, 20 mW for 1.0 s or 0.1 s) for de-condensation and self-diagnostics.
* **Unique Identification:** Read 32-bit hardware serial number from e-fuse.
* **Multi-Address Support:** Configurable I2C addressing (`0x44`, `0x45`, `0x46`).

---

## Wiring & Pinout

The SHT40 communicates over standard I2C (default 7-bit address: `0x44`).

| SHT40 Pin | MCU / Connection | Notes |
| :--- | :--- | :--- |
| **VDD** | 3.3V (1.08V – 3.6V) | Power supply |
| **GND** | GND | Ground |
| **SDA** | MCU SDA | Requires 4.7kΩ – 10kΩ pull-up |
| **SCL** | MCU SCL | Requires 4.7kΩ – 10kΩ pull-up |

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

### Initialization & Status
* `explicit KT_SHT40(TwoWire *wire = &Wire)`: Class constructor.
* `bool begin(TwoWire *wire = nullptr, kt_sht40_addr_enum addr = KT_SHT40_ADDR_0X44)`: Initializes sensor, checks bus presence, sets high precision, and prepares the rate limiter.
* `bool isConnected()`: Pings the sensor over I2C to verify ACK response.
* `bool reset()`: Triggers internal soft reset..
* `bool changeADDR(kt_sht40_addr_enum addr)`: Updates the target I2C address (`0x44`, `0x45`, `0x46`).

### Measurements
* `float getTemp()`: Returns ambient temperature in °C.
* `float getHum()`: Returns relative humidity in %RH.
* `bool setPrecision(kt_sht40_precision_enum value)`: Configures measurement repeatability:
  * `KT_SHT40_PRECISION_HIGH` (~10 ms delay)
  * `KT_SHT40_PRECISION_MEDIUM` (~6 ms delay)
  * `KT_SHT40_PRECISION_LOW` (~3 ms delay)

### Heater & Diagnostics
* `bool setHeaterMode(kt_sht40_heater_enum mode)`: Selects power and duration:
  * `KT_SHT40_HEATER_HIGH_LONG` (200 mW, 1s)
  * `KT_SHT40_HEATER_HIGH_SHORT` (200 mW, 0.1s)
  * `KT_SHT40_HEATER_MEDIUM_LONG` (110 mW, 1s)
  * `KT_SHT40_HEATER_MEDIUM_SHORT` (110 mW, 0.1s)
  * `KT_SHT40_HEATER_LOW_LONG` (20 mW, 1s)
  * `KT_SHT40_HEATER_LOW_SHORT` (20 mW, 0.1s)
* `bool setHeater()`: Executes the configured heating pulse and verifies response frame integrity.
* `uint32_t getSerialNumber()`: Returns the unique 32-bit hardware identifier.

### Debugging
* `void setDebugPort(Stream &debugPort)`: Assigns a serial stream (e.g., `Serial`) for error output.
* `void disableDebug()`: Disables text diagnostic messages.

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

* [Sensirion SHT4x Datasheet](https://sensirion.com/products/catalog/SHT40/)
* [Sensirion Application Note: Handling and De-condensation](https://sensirion.com/)

---

## License

This library is licensed under the **GNU General Public License v3.0**.