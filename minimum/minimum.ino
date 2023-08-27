//ATtiny85
//                     +-\/-+
//            RST PB5 1|*   |8 VCC
//                PB3 2|    |7 PB2
//WWVB ANTENNA <= PB4 3|    |6 PB1
//                GND 4|    |5 PB0
//                     +----+
//
//Arduino Nano
//       +----+=====+----+
//       |    | USB |    |
// SCK 13| D13+-----+D12 |12 MISO
//       |3V3        D11~|11 MOSI
//       |Vref       D10~|10 SS
//     14| A0/D14     D9~|9 => WWVB ANTENNA
//     15| A1/D15     D8 |8
//     16| A2/D16     D7 |7
//     17| A3/D17     D6~|6
// SDA 18| A4/D18     D5~|5
// SCL 19| A5/D19     D4 |4
//     20| A6         D3~|3 INT1
//     21| A7         D2 |2 INT0
//       | 5V        GND |
//       | RST       RST |
//       | GND       TX1 |0
//       | Vin       RX1 |1
//       |  5V MOSI GND  |
//       |   [ ][ ][ ]   |
//       |   [*][ ][ ]   |
//       | MISO SCK RST  |
//       +---------------+
//
//Arduino Micro
//       +--------+=====+---------+
//       |        | USB |         |
// SCK 13| D13    +-----+     D12 |12 MISO
//       |3V3                 D11~|11 MOSI
//       |Vref                D10~|10 SS
//     14| A0/D14              D9~|9 => WWVB ANTENNA
//     15| A1/D15              D8 |8
//     16| A2/D16              D7 |7
//     17| A3/D17              D6~|6
// SDA 18| A4/D18              D5~|5
// SCL 19| A5/D19              D4 |4
//     20| A6                  D3~|3 INT1
//     21| A7                  D2 |2 INT0
//       | 5V                 GND |
//       | RST                RST |
//       | GND                TX1 |0
//       | Vin                RX1 |1
//       |MISO MISO[*][ ]VCC    SS|
//       |SCK   SCK[ ][ ]MOSI MOSI|
//       |      RST[ ][ ]GND      |
//       +------------------------+
//
//Sparkfun Pro Micro
//                  +----+=====+----+
//                  |[J1]| USB |    |
//                 1| TXO+-----+RAW |
//                 0| RXI       GND |
//                  | GND       RST |
//                  | GND       VCC |
//             SDA 2| D2         A3 |21
//             SCL 3|~D3         A2 |20
//                 4| D4         A1 |19
//                 5|~D5         A0 |18
//                 6|~D6        D15 |15 SCK
//                 7| D7        D14 |14 MISO
//                 8| D8        D16 |16 MOSI
//WWVB ANTENNA  <= 9|~D9        D10~|10
//                  +---------------+

/*
Recommended debug setup
Use a RC (low-pass) filter to view the message (p1) as well as the modulated carrier (p0)

T = RC = 1/(2*pi*fc)
where;
T: Time constant
fc: cutoff frequency

for: R = 110 ohm, C = 1uF
fc = 1/(2*pi*RC) = 1.45kHz (the carrier is 60kHz, the modulation is about 1 to 5 Hz)

WWVB_OUT |--(p0)--[110R]--(p1)--[1uF]--|GND

-----------+-----------+-----------------
Chip       | #define   | WWVB_OUT
-----------+-----------+-----------------
ATtiny85   |  USE_OC1A | D1 / PB1 (pin 6)
ATtiny85   | *USE_OC1B | D4 / PB4 (pin 3)
-----------+-----------+-----------------
ATmega32u4 | *USE_OC1A | D9
ATmega32u4 |  USE_OC1B | D10
-----------+-----------+-----------------
ATmega328p | *USE_OC1A | D9
ATmega328p |  USE_OC1B | D10
-----------+-----------+-----------------

* Default setup
*/



#include <Arduino.h>
#include <avr/interrupt.h>
//#define WWVB_PAM

#if defined(__AVR_ATtiny25__) | defined(__AVR_ATtiny45__) | defined(__AVR_ATtiny85__)
#define _DEBUG 0
#elif defined(__AVR_ATmega16U4__) | defined(__AVR_ATmega32U4__)
#define _DEBUG 1
#else
#define _DEBUG 1
#endif

/*
_DEBUG == 0: Set wwvb time to the compile time, use the wwvb interrupt, blink the led
_DEBUG == 1: Set wwvb time to the compile time, use the wwvb interrupt, blink the led, serial output
*/

#if defined(__AVR_ATtiny25__) | defined(__AVR_ATtiny45__) | defined(__AVR_ATtiny85__)
unsigned char LED_PIN = 1; // Use the Tx LED on the digistump
#elif defined(__AVR_ATmega16U4__) | defined(__AVR_ATmega32U4__)
unsigned char LED_PIN = SS;
#elif defined(__AVR_ATmega168__) | defined(__AVR_ATmega168P__) | defined(__AVR_ATmega328P__)
unsigned char LED_PIN = 13;
#elif defined(__STM32F1__)
unsigned char LED_PIN = PB13;
#else
unsigned char LED_PIN = 13;
#endif

uint8_t mins = 0;
uint16_t LED_FAST = 100;
uint16_t SLOW_DELAY = 1000;
bool LED_TOGGLE = false;
uint32_t t0;


#define REQUIRE_TIMEDATESTRING 1

#include "TimeDateTools.h" // include before wwvb.h AND/OR ATtinyGPS.h
#include "wwvb.h" // include before ATtinyGPS.h

#include <Wire.h>
#include <Time.h>

wwvb wwvb_tx;

// The ISR sets the PWM pulse width to correspond with the WWVB bit
ISR(TIMER1_OVF_vect)
{
  wwvb_tx.interrupt_routine();
}


unsigned long T = 0;
int flag = 0;
time_t epochTime;
struct tm *current_time;

void setup()
{
  wwvb_tx.setup();

  Serial.begin(9600);

  Wire.begin(27);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println();  
}
void loop()
{
}

void receiveEvent(int howMany) {
  byte a, b, c, d;
  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();
  
  T = a;
  T = (T << 8) | b;
  T = (T << 8) | c;
  T = (T << 8) | d;

  epochTime = T;
  current_time = localtime(&epochTime);

  Serial.println(T);
    uint8_t hh = current_time->tm_hour + 2;
    uint8_t mm = current_time->tm_min;
    uint8_t ss = current_time->tm_sec;
    uint8_t DD = current_time->tm_mday + 1;
    uint8_t MM = current_time->tm_mon + 1;
    uint8_t YY = current_time->tm_year - 230;

    wwvb_tx.set_time(hh, mm, DD, MM, YY, 0);
    wwvb_tx.debug_time();
    wwvb_tx.start(); // Thats it
}
