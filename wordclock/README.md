# Word Clock
This is an ESP controlled word clock based on a few different open source projects. I'm including the custom source code here, while the laser cut files and assembly instructions are available elsewhere

The source is based off of https://github.com/leinich/ha-wordclock-esphome

The clock face and design is based off of https://github.com/mattudakis/ArduinoLEDClock

## Clock Face Lettering
```
GITSISMRUKB  110 .......... 120
HEAQUARTERI  109 .......... 99
ATWENTYIENR   88 .......... 98
PHALFRFIVET   87 .......... 77
PBAEYJPASTH   66 .......... 76
YTOCTWELVED   65 .......... 55
FIVEIGHTENA   44 .......... 54
PTWONENINEY   43 .......... 33
DTHREELEVEN   22 .......... 32
TEFOURSEVEN   21 .......... 11
JSIXJOCLOCK   0 ........... 10
```
## File Layout
The sensor source code files will need to be put in home assistant at the following path. This can be done using the Visual Studio Code addon for home assistant which provides a nice IDE to use.
- /config/esphome/wordclock.h

## Wiring
 * GPIO 2 (D4) -> LED Strip Signal
 * GPIO 17 (A0) -> Analog Light Sensor Input
 * GPIO 4 (D2) -> Real time clock SDA
 * GPIO 5 (D1) -> Real time clock SCL

# To Do:
- ~~add a color selection option through HASS to control clock face color~~
- save the recent color and use it on boot?
- ~~fix the minutes~~
- add the happy birthday lettering
- create input boolean from HASS to trigger the lettering or turn it off
- add some flair to the text when it turns to the hour
- 
