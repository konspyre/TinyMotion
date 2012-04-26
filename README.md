TinyMotion
==========

A homebrew, remote motion detector that tries to slowly sip power.

### Goals and motivations

TinyMotion was designed out of parts lying around; mostly out of a desire to use
some protoboard and play with AVR power saving features.

After much nightly wrangling the source and schematic are now up for anybody else
to learn from (which probably include mistakes).

### Features

* Remote motion monitoring with 8 second heartbeat (using the watchdog timer) 
* Power saving sleep: Both XBee (w/pin hibernate) and ATTiny85 can sleep when there is nothing to do
* Chip can be programmed using the familiar [Arduino environment](http://hlt.media.mit.edu/?p=1695)

For more information about power usage, fuses, clock and pin connections check out the 
[source](https://github.com/davidk/TinyMotion/blob/master/TinyMotion.ino).

For a wiring diagram, there is a crude schematic [here](https://github.com/davidk/TinyMotion/raw/master/schematic.png).

### Parts list

* [ATTiny85](http://www.mouser.com/ProductDetail/Atmel/ATtiny85-20PU/?qs=8jWQYweyg6NCiiaOb5GI9Q%3d%3d)
* [PIR sensor](http://www.adafruit.com/products/189)
* [2x4 DIP socket](http://search.digikey.com/scripts/dksearch/dksus.dll?vendor=0&keywords=3M5461-ND)
* [Header](https://www.adafruit.com/products/400)
* [Female-Female jumper wiring](https://www.adafruit.com/products/266)
* 2x [XBee adapter kit](https://www.adafruit.com/products/126)
* 2x [XBee series 1](https://www.adafruit.com/products/128)    
* [Protoboard](https://www.adafruit.com/products/589)
* [Screw terminal](https://www.adafruit.com/products/724)
* [3mm Green LED](https://www.adafruit.com/products/779)
* [Switch button](https://www.adafruit.com/products/367)
* 1x330 ohm (LED), 1x10k ohm (switch pull-down), 1x100ohm (switch-pin buffer) resistors
* [5v power source](https://www.adafruit.com/products/14)

### Notes on XBee

Pin hibernate must be enabled on the XBee to realize real power savings, as it is the largest
consumer of power in the project. Under power options in X-CTU, enable `Pin Hibernate`. The
ATtiny chip will wake up the XBee when it needs to transmit data.
