/*
 * TinyMotion - ATtiny85 + PIR + XBee + Lipoly battery + other stuff with a long run time
 * (In theory of course)
 * 
 * Fuses programmed for AVR:
 * LFUSE = 0xE2 (Low Fuse)
 * HFUSE = 0xDF (High Fuse)
 * EFUSE = 0xFF (Extended Fuse)
 * 
 * Clock: Internal 8Mhz
 * Vcc: 5V from Adafruit MintyBoost connected to LiPoly battery
 * Attached hardware: PIR sensor (detection), XBee (notification), 3mm Green LED (notification), 
 * Push button (test)
 * 
 * Run-time test calculations: http://oregonembedded.com/batterycalc.htm
 * Battery capacity: 1300mAh LiPoly battery
 * Current consumption duing sleep: 0.30mA
 * Current consumption during wakeup: 30mA
 * Number of wakeups per hour: 450 (watchdog triggered every ~8 seconds, 7.5 triggers per minute, 450 triggers
 *  per hour) .. 60 [seconds/per minute] / 8 [1 trigger/every 8 seconds] = 7.5 [triggers/min]. 
 *               7.5 [triggers/minute] * 60 [minutes/hr] = 450 [triggers/hour].
 * Duration of wake: around 55.4ms
 * After 15% derating for self-discharge, 91.32 days or 0.25 years 
 * (3 months without accounting for motion sensed, DC conversion loss, subatomic physics, etc)
 * 
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include <SoftwareSerial.h>

#define CHIPID "DECAFBAD"          // A unique identifier for this particular chip. 
                                   // Change this if you have multiple monitors.

SoftwareSerial xBeeComm(NULL, 3);  // None == RX, 3 == TX

const uint8_t ledPin      = 0;     // choose the pin for the LED
const uint8_t inputPin    = 2;     // choose the input pin (for PIR sensor)
const uint8_t xBeeWakePin = 1;     // xbee wake up pin
                                   // configured with 'pin hibernate' (modem configuration 
                                   // -> sleep modes in X-CTU)
const uint8_t switchPin   = 4;     // test switch (if the PIR is broken this can help check its state)
bool wokenByDog = 0;               // state flag for watchdog timer wakeup

void setup() {
  // Wake up at regular intervals and send information
  //WDTCR |= _BV(WDE); // enable watchdog timer
  // enable watchdog timer in interrupt mode (using an 8 second timeout -- see p. 47-49 of datasheet)
  WDTCR |= _BV(WDIE) | _BV(WDP3) | _BV(WDP0);

  // Normally this interrupt is the right way to do stuff, but it seems to make softwareserial unhappy.
  // Instead, this uses pin change interrupts, which catch on both the rising and falling edge
  // of the PIR in edge triggering mode (so it alarms 'twice').
  
  // If you feel like hacking and slashing your way to a fix, here is how to setup INT0 on pin 2:
  // GIMSK |= _BV(INT0);   // Enable INT0 external interrupt (pin 2)
  // MCUCR |= _BV(ISC01) | _BV(ISC01); // Trigger interrupt on rising edge

  // Pin change interrupts
  // See 9.3.2 General Interrupt Mask Register (ATtiny85 data sheet) for information on this
  GIMSK |= _BV(PCIE);   // Enable pin change interrupts
  PCMSK |= _BV(PCINT2); // Wake on PIR pin change
  PCMSK |= _BV(PCINT4); // Wake on switch press, pin 4
  
  // Power conservation
  ADCSRA &= ~_BV(ADEN);    // Disable ADC
  ACSR |= _BV(ACD);        // Disable the analog comparator
  DIDR0 |= _BV(ADC0D) | _BV(ADC2D) | _BV(ADC3D) | _BV(ADC1D) 
            | _BV(AIN1D) | _BV(AIN0D); // digital input buffer on ADC pins disabled

  xBeeComm.begin(9600);
  pinMode(ledPin, OUTPUT); 
  pinMode(inputPin, INPUT);
  pinMode(xBeeWakePin, OUTPUT);
  pinMode(switchPin, INPUT);
}
 
void loop() {  
  systemWakeup();

  // Heartbeat
  if(wokenByDog) {
    xBeeComm.print(CHIPID);
    xBeeComm.print(':');
    xBeeComm.println("STILL_ALIVE");
    wokenByDog = 0;
  }
  
  // PIR sensed something
  if(digitalRead(inputPin) == HIGH) {
    xBeeComm.print(CHIPID);
    xBeeComm.print(':');
    xBeeComm.println("ALARM");
  }  

  // Chip test
  if(digitalRead(switchPin) == HIGH) {
    xBeeComm.print(CHIPID);
    xBeeComm.print(':');
    xBeeComm.println("CHIP_OK"); 
  }
  
  systemSleep(); 
}

void systemWakeup() {
   // Turn radio ON (pull low to GND for on)
  digitalWrite(xBeeWakePin, LOW);    
  // Turn on threatening 3mm LED
  digitalWrite(ledPin, HIGH);
  // Wait for radio to power up
  delay(15); 
}

void systemSleep() {
  delay(15); // Wait for any remaining transmissions to finish
  digitalWrite(ledPin, LOW);
  digitalWrite(xBeeWakePin, HIGH);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
}

ISR(WDT_vect) {
  wokenByDog = 1;
}
