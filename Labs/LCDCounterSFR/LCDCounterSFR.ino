// C++ code
//
/*
  LiquidCrystal Library - Hello World

   Demonstrates the use of a 16x2 LCD display.
  The LiquidCrystal library works with all LCD
  displays that are compatible with the  Hitachi
  HD44780 driver. There are many of them out
  there, and you  can usually tell them by the
  16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>

// GREEN
//#define LED_GREEN pin 6 -> H3
#define REG_DDR_LED_G DDRH
#define REG_PORT_LED_G PORTH
#define REG_PIN_LED_G PINH
#define BIT_LED_G 3

//#define PB_G pin 2 -> D2
#define REG_DDR_PB_G DDRD
#define REG_PORT_PB_G PORTD
#define REG_PIN_PB_G PIND
#define BIT_PB_G 2

// YELLOW
//#define PIN_LED_YELLOW pin 7
#define REG_DDR_LED_Y DDRH
#define REG_PORT_LED_Y PORTH
#define REG_PIN_LED_Y PINH
#define BIT_LED_Y 4

//#define PIN_PB_YELLOW pin 3
#define REG_DDR_PB_Y DDRD
#define REG_PORT_PB_Y PORTD
#define REG_PIN_PB_Y PIND
#define BIT_PB_Y 3

// RESET
//#define PIN_PB_RESET pin 4
#define REG_DDR_PB_RST DDRD
#define REG_PORT_PB_RST PORTD
#define REG_PIN_PB_RST PIND
#define BIT_PB_RST 0

// int seconds = 0;

LiquidCrystal lcd_1(13, 12, 11, 10, 9, 8);

volatile uint8_t gCount = 0;
volatile uint8_t yCount = 0;
volatile uint8_t mainEventFlag = 0;
#define PB_G_FLAG 0x01
#define PB_Y_FLAG 0x02
#define PB_RST_FLAG 0x04

void setup() {
  Serial.begin(9600);
  // setup LED pins
  REG_DDR_LED_G |= _BV(BIT_LED_G);
  REG_DDR_LED_Y |= _BV(BIT_LED_Y);
  // setup buttons
  REG_DDR_PB_G &= ~_BV(BIT_PB_G);
  REG_DDR_PB_Y &= ~_BV(BIT_PB_Y);
  REG_DDR_PB_RST &= ~_BV(BIT_PB_RST);
  // setup pullups
  REG_PORT_PB_G |= _BV(BIT_PB_G);
  REG_PORT_PB_Y |= _BV(BIT_PB_Y);
  REG_PORT_PB_RST |= _BV(BIT_PB_RST);
  // setup interupprs
  // GREEN
  EIMSK |= _BV(INT2);
  EICRA |= _BV(ISC21);   // falling edge setup
  EICRA &= ~_BV(ISC20);  // falling edge setup
  // YELLOW
  EIMSK |= _BV(INT3);
  EICRA |= _BV(ISC31);   // falling edge setup
  EICRA &= ~_BV(ISC30);  // falling edge setup
  // RESET
  EIMSK |= _BV(INT0);
  EICRA |= _BV(ISC01);   // falling edge setup
  EICRA &= ~_BV(ISC00);  // falling edge setup
  // set up LCD
  lcd_1.begin(16, 2);        // Set up the number of columns and rows on the LCD.
  lcd_1.print("Ben Salak");  // Print a message to the LCD.
}

void loop() {
  // GREEN
  if (mainEventFlag & PB_G_FLAG) {
    delay(20);
    // Serial.println("tik1");
    mainEventFlag &= ~PB_G_FLAG;
    if (bit_is_clear(REG_PIN_PB_G, BIT_PB_G)) {
      // Serial.println("tik2");
      gCount++;
      REG_PORT_LED_G |= _BV(BIT_LED_G);
      REG_PORT_LED_Y &= ~_BV(BIT_LED_Y);
      // Serial.println(String(gCount));
    }
  }

  // YELLOW
  if (mainEventFlag & PB_Y_FLAG) {
    delay(20);
    mainEventFlag &= ~PB_Y_FLAG;
    // Serial.println("tik1");
    if (bit_is_clear(REG_PIN_PB_Y, BIT_PB_Y)) {
      // Serial.println("tik2");
      yCount++;
      REG_PORT_LED_Y |= _BV(BIT_LED_Y);
      REG_PORT_LED_G &= ~_BV(BIT_LED_G);
      // Serial.println(String(gCount));
    }
  }

  // RESET
  if (mainEventFlag & PB_RST_FLAG) {
    delay(20);
    mainEventFlag &= ~PB_RST_FLAG;
    // Serial.println("tik1");
    if (bit_is_clear(REG_PIN_PB_RST, BIT_PB_RST)) {
      // Serial.println("tik2");
      REG_PORT_LED_G &= ~_BV(BIT_LED_G);
      REG_PORT_LED_Y &= ~_BV(BIT_LED_Y);
      gCount = 0;
      yCount = 0;
      // seconds = 0;
    }
  }

  updateLCD();
  // delay(100); // to help simulation speed
}

void updateLCD() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting
  // begins with 0):
  lcd_1.setCursor(0, 1);
  lcd_1.print("G=");
  lcd_1.print(gCount);

  lcd_1.print("  Y=");
  lcd_1.print(yCount);

  lcd_1.print("  T=");
  // lcd_1.print(seconds);// print the number of seconds since reset:
  // delay(1000); // Wait for 1000 millisecond(s)
  // seconds += 1; THE EXAMPLE CODE HAS CHANGED
  lcd_1.print(millis() / 1000);
}

ISR(INT0_vect) {  // weird func (more like macro), in documentation somewhere
  // gCount++;
  //Serial.println(String(mainEventFlag));
  mainEventFlag |= PB_RST_FLAG;
  // Serial.println("pushed");
  //Serial.println(String(mainEventFlag));
}
//
ISR(INT2_vect) {  // weird func (more like macro), in documentation somewhere
  // gCount++;
  //Serial.println(String(mainEventFlag));
  mainEventFlag |= PB_G_FLAG;
  //Serial.println(String(mainEventFlag));
  // Serial.println("pushed");
}
//
ISR(INT3_vect) {  // weird func (more like macro), in documentation somewhere
  // gCount++;
  // Serial.println("pushed");
  //Serial.println(String(mainEventFlag));
  mainEventFlag |= PB_Y_FLAG;
  //Serial.println(String(mainEventFlag));
}
//
