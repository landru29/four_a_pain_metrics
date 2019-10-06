# Push temperatures to OVH metrics timeseries

## Prerequisite

* Have a WEMOS D1 Mini
* Have a sane installation of Arduino IDE
* Have WEMOS toolkit installed (<https://github.com/esp8266/Arduino>)

## Hardware

* Wemos D1 Mini
* MAX31855
* K type Thermocouple

All these items can be found on <https://aliexpress.com>

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

## config.h

The file shold contain the following definition:

```c
#define TOKEN "xxxx"
```

The token is the write token you created in the OVH manager (Cloud > metrics): <https://www.ovh.com/manager/cloud/#/>

## Fingerprint

in `http.cpp`, you must check to have the correct fingerprint:

```bash
echo | openssl s_client -showcerts -servername opentsdb.gra1.metrics.ovh.net -connect opentsdb.gra1.metrics.ovh.net:443 2>/dev/null | openssl x509 -noout -fingerprint -sha1 -inform pem | sed -e "s/.*=//g" | sed -e "s/\:/ /g"
```

## VSCode

Install `Arduino` extension

Create file `.vscode/c_cpp_properties.json`

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${env:HOME}/Arduino/hardware/esp8266com/esp8266/variants/d1_mini",
                "${env:HOME}/Arduino/hardware/esp8266com/esp8266/**",
                "${env:HOME}/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/libraries/**",
                "${env:HOME}/Arduino/libraries/**",
            ],
            "forcedInclude": [],
            "intelliSenseMode": "gcc-x64",
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c11",
            "cppStandard": "c++17"
        }
    ],
    "version": 4
}
```

Create file `.vscode/arduino.json`

```json
{
    "board": "esp8266com:esp8266:d1_mini",
    "configuration": "xtal=80,vt=flash,exception=legacy,ssl=all,eesz=4M2M,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=921600",
    "sketch": "four_a_pain_metrics.ino",
    "port": "/dev/ttyUSB0"
}
```
