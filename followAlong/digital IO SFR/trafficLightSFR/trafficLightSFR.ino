// C++ code
//

//#define PIN_LED_RED 6
#define REG_DDR_LED_RED DDRH
#define REG_PORT_LED_RED PORTH
#define REG_PIN_LED_RED PINH
#define BIT_LED_RED 3

//#define PIN_LED_YELLOW 5
#define REG_DDR_LED_YELLOW DDRE
#define REG_PORT_LED_YELLOW PORTE
#define REG_PIN_LED_YELLOW PINE
#define BIT_LED_YELLOW 3

//#define PIN_LED_GREEN 4
#define REG_DDR_LED_GREEN DDRG
#define REG_PORT_LED_GREEN PORTG
#define REG_PIN_LED_GREEN PING
#define BIT_LED_GREEN 5

//#define PIN_PUSHBUTTON 2
#define REG_DDR_PUSHBUTTON DDRE
#define REG_PORT_PUSHBUTTON PORTE
#define REG_PIN_PUSHBUTTON PINE
#define BIT_PUSHBUTTON 4

void setup()
{
  //pinMode(PIN_LED_RED, OUTPUT);
  REG_DDR_LED_RED |= _BV(BIT_LED_RED);
  
  //pinMode(PIN_LED_YELLOW, OUTPUT);
  REG_DDR_LED_YELLOW |= _BV(BIT_LED_YELLOW);
  
  //pinMode(PIN_LED_GREEN, OUTPUT);
  REG_DDR_LED_GREEN |= _BV(BIT_LED_GREEN);
  
  //pinMode(PIN_PUSHBUTTON, INPUT_PULLUP);
  REG_DDR_PUSHBUTTON &= ~_BV(BIT_PUSHBUTTON); // not strictly needed b/c input default
  REG_PORT_PUSHBUTTON |= _BV(BIT_PUSHBUTTON); // want pullup to be on
  

}

void loop()
{
  //(REG_PIN_PUSHBUTTON >> BIT_PUSHBUTTON) & 0x01 // gets rid of bits below, then gets rid of all bits above
    
    
  if (bit_is_clear(REG_PIN_PUSHBUTTON, BIT_PUSHBUTTON)){
    //digitalWrite(PIN_LED_RED, LOW);
    REG_PORT_LED_RED &= ~_BV(BIT_LED_RED);
    //digitalWrite(PIN_LED_YELLOW, LOW);
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    //digitalWrite(PIN_LED_GREEN, HIGH);
    REG_PORT_LED_GREEN |= _BV(BIT_LED_GREEN);
  	delay(1000); 
    //digitalWrite(PIN_LED_RED, LOW);
    REG_PORT_LED_RED &= ~_BV(BIT_LED_RED);
    //digitalWrite(PIN_LED_YELLOW, HIGH);
    REG_PORT_LED_YELLOW |= _BV(BIT_LED_YELLOW);
    //digitalWrite(PIN_LED_GREEN, LOW);
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  	delay(1000); 
    //digitalWrite(PIN_LED_RED, HIGH);
    REG_PORT_LED_RED |=_BV(BIT_LED_RED);
    //digitalWrite(PIN_LED_YELLOW, LOW);
    REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
    //digitalWrite(PIN_LED_GREEN, LOW);
    REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  	}
  
  //digitalWrite(PIN_LED_RED, HIGH);
  REG_PORT_LED_RED |=_BV(BIT_LED_RED);
  //digitalWrite(PIN_LED_YELLOW, LOW);
  REG_PORT_LED_YELLOW &= ~_BV(BIT_LED_YELLOW);
  //digitalWrite(PIN_LED_GREEN, LOW);
  REG_PORT_LED_GREEN &= ~_BV(BIT_LED_GREEN);
  delay(10);
    
  }