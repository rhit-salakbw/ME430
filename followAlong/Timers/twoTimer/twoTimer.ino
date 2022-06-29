#include <LiquidCrystal.h>

/***  LCD ***/
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define LINE_1 0
#define LINE_2 1

//timing planner

//green timer to have interupt every .001 w/ timer 2

//TIMSK 
//OCIE2A high 
// will enable interrupt

//  need prescaler 
//TCCR2B 
// resulting ticks must be under 255 b/c timer is only 8 bit 
//options : 
//1 -> .001 / (1/ 6 000 0000) = 16000 ticks for .001s
//8 -> 2000 ticks 
//32 -> 500 ticks 
//64 -> 250 ticks (GOOD)
//128 ->
//256 ->
//1024 ->
#define TIMER_2_START  0
#define TIMER_2_END  250

//yellow timer to have interrupt every .1 w/ timer 1
// timer is 16 bit! (needs to be less than 65,535!
//1 -> .1 / (1/ 16 000 0000) = 1600000 ticks for .1s
//8 -> 200000 ticks 
//64 -> 25000 ticks
//256 ->
//1024 ->
#define TIMER_1_START  0
#define TIMER_1_END  25000


// Uno
/*
#define REG_DDR_PUSHBUTTON_GREEN DDRD
#define REG_PORT_PUSHBUTTON_GREEN PORTD
#define REG_PIN_PUSHBUTTON_GREEN PIND
#define BIT_PUSHBUTTON_GREEN 2

#define REG_DDR_PUSHBUTTON_YELLOW DDRD
#define REG_PORT_PUSHBUTTON_YELLOW PORTD
#define REG_PIN_PUSHBUTTON_YELLOW PIND
#define BIT_PUSHBUTTON_YELLOW 3

#define REG_DDR_PUSHBUTTON_RESET DDRD
#define REG_PORT_PUSHBUTTON_RESET PORTD
#define REG_PIN_PUSHBUTTON_RESET PIND
#define BIT_PUSHBUTTON_RESET 4

#define REG_DDR_LED_GREEN DDRD
#define REG_PORT_LED_GREEN PORTD
#define REG_PIN_LED_GREEN PIND
#define BIT_LED_GREEN 6

#define REG_DDR_LED_YELLOW DDRD
#define REG_PORT_LED_YELLOW PORTD
#define REG_PIN_LED_YELLOW PIND
#define BIT_LED_YELLOW 7
*/
// Mega

#define REG_DDR_PUSHBUTTON_GREEN DDRD
#define REG_PORT_PUSHBUTTON_GREEN PORTD
#define REG_PIN_PUSHBUTTON_GREEN PIND
#define BIT_PUSHBUTTON_GREEN 2

#define REG_DDR_PUSHBUTTON_YELLOW DDRD
#define REG_PORT_PUSHBUTTON_YELLOW PORTD
#define REG_PIN_PUSHBUTTON_YELLOW PIND
#define BIT_PUSHBUTTON_YELLOW 3

#define REG_DDR_PUSHBUTTON_RESET DDRD
#define REG_PORT_PUSHBUTTON_RESET PORTD
#define REG_PIN_PUSHBUTTON_RESET PIND
#define BIT_PUSHBUTTON_RESET 0

#define REG_DDR_LED_GREEN DDRH
#define REG_PORT_LED_GREEN PORTH
#define REG_PIN_LED_GREEN PINH
#define BIT_LED_GREEN 3

#define REG_DDR_LED_YELLOW DDRH
#define REG_PORT_LED_YELLOW PORTH
#define REG_PIN_LED_YELLOW PINH
#define BIT_LED_YELLOW 4

bool isGreenTimerOn = false;
volatile uint16_t greenCounterMillis = 0;

bool isYellowTimerOn = false;
volatile uint16_t yellowCounterTen = 0;

//I write code for yellow timer 

/*** Interrupt flags ***/
volatile int mainEventFlags = 0;
#define FLAG_PUSHBUTTON_GREEN 0x01
#define FLAG_PUSHBUTTON_YELLOW 0x02

#define DEBOUNCE_TIME_MS 20
volatile uint16_t debounceCounter = 0;

void setup() {
  lcd.begin(16, 2);
  
  // Setup the pushbuttons as inputs (the default)
  REG_DDR_PUSHBUTTON_GREEN &= ~_BV(BIT_PUSHBUTTON_GREEN);
  REG_DDR_PUSHBUTTON_YELLOW &= ~_BV(BIT_PUSHBUTTON_YELLOW);
  REG_DDR_PUSHBUTTON_RESET &= ~_BV(BIT_PUSHBUTTON_RESET);
  
  // Enable pull-up resistors on the inputs
  REG_PORT_PUSHBUTTON_GREEN |= _BV(BIT_PUSHBUTTON_GREEN);
  REG_PORT_PUSHBUTTON_YELLOW |= _BV(BIT_PUSHBUTTON_YELLOW);
  REG_PORT_PUSHBUTTON_RESET |= _BV(BIT_PUSHBUTTON_RESET);  

  // Setup the LEDs as outputs
  REG_DDR_LED_GREEN |= _BV(BIT_LED_GREEN);
  REG_DDR_LED_YELLOW |= _BV(BIT_LED_YELLOW);

  // Make sure the LEDs start off (the default)
  REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);

  // Uno
  // Setup 2 interrupts
  /*
  EIMSK |= _BV(INT0); // Turns on INT0 the external interrupt
  EICRA |= _BV(ISC01); // part of FALLING edge setup
  EICRA &= ~_BV(ISC00); // part of FALLING edge setup
  
  EIMSK |= _BV(INT1); // Turns on INT1 the external interrupt
  EICRA |= _BV(ISC11); // part of FALLING edge setup
  EICRA &= ~_BV(ISC10); // part of FALLING edge setup
  */

  // Mega
  // Setup 3 interrupts
  
  EIMSK |= _BV(INT0); // Turns on INT0 the external interrupt
  EICRA |= _BV(ISC01); // part of FALLING edge setup
  EICRA &= ~_BV(ISC00); // part of FALLING edge setup
  
  EIMSK |= _BV(INT2); // Turns on INT2 the external interrupt
  EICRA |= _BV(ISC21); // part of FALLING edge setup
  EICRA &= ~_BV(ISC20); // part of FALLING edge setup
  
  EIMSK |= _BV(INT3); // Turns on INT3 the external interrupt
  EICRA |= _BV(ISC31); // part of FALLING edge setup
  EICRA &= ~_BV(ISC30); // part of FALLING edge setup

  
  //Timer Interrupts
  //GREEN
  TIMSK2 |= _BV(OCIE2A);
  TCCR2A = 0; //get rid of arduino functions messing around 
  TCCR2B |= _BV(CS22);
  TCCR2B &= ~_BV(CS21);
  TCCR2B &= ~_BV(CS20);
  
  TCNT2 = TIMER_2_START;
  OCR2A = TIMER_2_END;
  //YELLOW
  TIMSK1 |= _BV(OCIE1A);
  TCCR1A = 0; //get rid of arduino functions messing around 
  TCCR1B &= ~_BV(CS12);
  TCCR1B |= _BV(CS11);
  TCCR1B |= _BV(CS10);
  
  TCNT1 = TIMER_1_START;
  OCR1A = TIMER_1_END;
  
  
  sei();  // Turn on interrupts!  Do this last.
}

void loop() {
  updateLcd();
  
  if (mainEventFlags & FLAG_PUSHBUTTON_GREEN) {
    blockDelay(DEBOUNCE_TIME_MS);
    mainEventFlags &= ~FLAG_PUSHBUTTON_GREEN;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_GREEN, BIT_PUSHBUTTON_GREEN)) {
    	// Do stuff - toggle LED
        isGreenTimerOn = !isGreenTimerOn;
        REG_PORT_LED_GREEN ^= _BV(BIT_LED_GREEN);
      	
    }    
  }
  if (mainEventFlags & FLAG_PUSHBUTTON_YELLOW) {
    blockDelay(DEBOUNCE_TIME_MS);
    mainEventFlags &= ~FLAG_PUSHBUTTON_YELLOW;
    if (bit_is_clear(REG_PIN_PUSHBUTTON_YELLOW, BIT_PUSHBUTTON_YELLOW)) {
		//do stuff    
        isYellowTimerOn = !isYellowTimerOn;
        REG_PORT_LED_YELLOW ^= _BV(BIT_LED_YELLOW);
    }
  }
  
  // Uno
  /*
  if (bit_is_clear(REG_PIN_PUSHBUTTON_RESET, BIT_PUSHBUTTON_RESET)) {
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    isGreenTimerOn = false;
    greenCounterMillis = 0; 
    isYellowTimerOn = false;
  }
  */
  //delay(100); // Help the simulation a lot
}

void updateLcd() {
  lcd.setCursor(0, LINE_1);
  uint8_t greenCounterSec = greenCounterMillis / 1000;
  uint8_t greenCounterTen = (greenCounterMillis / 100 ) % 10;
  lcd.print(greenCounterSec);
  lcd.print(".");
  lcd.print(greenCounterTen);
  lcd.print("     ");

  //1234
  // want 1.2 
  //1234/100 -> mod(10)
  if (isYellowTimerOn){
    lcd.print("          ");
  }else {
  	lcd.setCursor(0, LINE_2);
  	uint8_t yellowCounterSec = yellowCounterTen / 10;
  	uint8_t yellowCounterTenPlace = yellowCounterTen % 10;
  	lcd.print(yellowCounterSec);
  	lcd.print(".");
  	lcd.print(yellowCounterTenPlace);
  	lcd.print("     ");
  }
}

void blockDelay(uint16_t delayMillis){
  debounceCounter = delayMillis;
  while (debounceCounter > 0){
    //do nothing
  }
}



// Simple ISRs that set flags only

// Uno
/*
ISR(INT0_vect) {
  mainEventFlags |= FLAG_PUSHBUTTON_GREEN;
}
ISR(INT1_vect) {
  mainEventFlags |= FLAG_PUSHBUTTON_YELLOW;
}
*/
// Mega

ISR(INT0_vect) {
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    isGreenTimerOn = false;
    greenCounterMillis = 0; 
    isYellowTimerOn = false;
    yellowCounterTen = 0; // not sure if this is needed 
}
ISR(INT2_vect) {
  mainEventFlags |= FLAG_PUSHBUTTON_GREEN;
}
ISR(INT3_vect) {
  mainEventFlags |= FLAG_PUSHBUTTON_YELLOW;
}

//Timer stuff 
ISR(TIMER2_COMPA_vect){
  TCNT2 = TIMER_2_START;
  if (isGreenTimerOn){
    greenCounterMillis++;
  }
  
  if (debounceCounter > 0){
    debounceCounter--;
  }
}

ISR(TIMER1_COMPA_vect){
  TCNT1 = TIMER_1_START;
  if (isYellowTimerOn){
    yellowCounterTen++;
  }
}
 