# Push temperatures to OVH metrics timeseries

## Prerequisite

* Have a WEMOS D1 Mini
* Have a sane installation of Arduino IDE
* Have WEMOS toolkit installed (https://github.com/esp8266/Arduino)

## Schema

Install Fritzing to display schema

MAX31855 is connected to Wemos:

* Wemos `D5` <-> MAX31855 `CLK`
* Wemos `D6` <-> MAX31855 `CS`
* Wemos `D7` <-> MAX31855 `DO`
* Wemos `GND` <-> MAX31855 `GND`
* Wemos `5V` <-> MAX31855 `VCC`

## Arduino

* Create a file `config.h` based on `config.h.sample`
* Compile program
²