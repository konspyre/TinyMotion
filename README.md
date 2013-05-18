TinyMotion
==========

A battery powered, remote motion sensor.

Note: A PCB version of TinyMotion is being planned, but feel free to make your own if you wish! (After all,
the PCB might take awhile.)

### Features

* 8 second heartbeat (periodic check-ins let you know if the chip is still alive and kicking).
* Low power: Properly configured, all components use very little power until woken up either by the heartbeat check-in or by physical intervention (motion/test button).
* Can be programmed using the familiar [Arduino environment](http://hlt.media.mit.edu/?p=1695).

### Power

With a 1300mAh LiPoly battery (upped to 5v by a MintyBoost), TinyMotion might stay on
for about 113.45 days, or just around 3.72 months. Real world conditions 
cause all sorts of degradation in run time. Expect (conservatively) that TinyMotion will
last you a few weeks at best. (No promises!)

Here are some stats:

* Idle power: ~0.06mA

* Wake up (every 8 seconds or when motion is detected/button is pressed): ~50mA (approximate)

* Wake-ups per hour: 450

* Vcc: 5v (running at a lower voltage will reduce power consumption, but requires bypassing the regulator on the PIR sensor)

For more information on fuses, power usage, clock, and pin connections check out the 
[source](https://github.com/davidk/TinyMotion/raw/master/TinyMotion.ino).

### Building on your breadboard

To build your own TinyMotion, get all the parts from the parts list and hook everything
up as like [this](https://github.com/davidk/TinyMotion/raw/master/breadboard.png).

Be sure the set the PIR's re-triggering (the jumper/shunt on the back) to 'L'. By default it is set to 'H'. It will 
still work when set to 'H', so you may want to experiment with what works for you.

For a wiring diagram, there is a crude schematic [here](https://github.com/davidk/TinyMotion/raw/master/schematic.png).

### Making things permanent

TinyMotion's parts will fit on a 1/4 sized Adafruit perma-proto (except for the PIR and XBee). Sizing is 1.7" (long) x 2" (high).
[Here](https://github.com/davidk/TinyMotion/raw/master/board.png) is an image of how it looks when assembled. The trace
between the LED and switch has been cut underneath the board.

### Parts list

The following parts are needed to build up a working TinyMotion implementation.

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
* Arduino (you will need a 10uF cap for the reset line) or other AVR programmer

### Programming

#### If using Arduino

Follow these instructions to setup the [Arduino environment](http://hlt.media.mit.edu/?p=1695) so that your ATTiny85 chip can be programmed. Be sure (and its super important!) to burn the 8MHz bootloader onto your ATTiny85. This is required for SoftwareSerial to work. Otherwise all the data coming through on the remote end will be gibberish, or blank.

To program, copy the [TinyMotion.ino](https://github.com/davidk/TinyMotion/raw/master/TinyMotion.ino)'s file contents into a new sketch and just upload it.

#### Dedicated AVR programmer

Use the [TinyMotion.hex](https://github.com/davidk/TinyMotion/raw/master/TinyMotion.hex) file to upload. Be sure to burn your fuses to enable the 8Mhz clock and other features. 

The fuse settings can be found at the top of the source file [here](https://github.com/davidk/TinyMotion/raw/master/TinyMotion.ino).

### Notes on XBee

Pin hibernate must be enabled (**only**) on the TinyMotion's XBee to save power. 
Under `Sleep Modes (NonBeacon)` in X-CTU, select `1 - PIN HIBERNATE` in 
the `(0) SM - Sleep Mode` category. Click 'Write' to finalize the change.

If this is your first time setting up an XBee, try Adafruit's helpful guide 
which walks you through the setup process. It can be found [here](http://www.ladyada.net/make/xbee/configure.html).

The ATtiny chip will wake up the XBee when it needs to transmit data by changing the DTR pin.
