// C++ code
//
// NOTE: pin 6 is PH3 on ATmega2560
//LED builtin is PB 7

//DDR:
//PORT:B
//PIN: B -> output so not needed

//#define REG_DDR_LED DDRB
//#define REG_PORT_LED PORTB
//#define REG_PIN_LED PINB

#define REG_DDR_LED DDRH
#define REG_PORT_LED PORTH
#define REG_PIN_LED PINH

#define BIT_LED 3

void setup()
{
  //pinMode(13, OUTPUT);
  //DDRB |= 0b00100000 //set pin 13 as output -> use or equal to  oinly effect the one we care about 
  //DDRB |= (1<<5);
  //DDRB |= _BV(5); -> using macro
    
  REG_DDR_LED |= _BV(BIT_LED);
    
}

void loop()
{
  //digitalWrite(13, HIGH);
  REG_PORT_LED |= _BV(BIT_LED);
  delay(1000);
  //digitalWrite(13, LOW);
  REG_PORT_LED &= ~_BV(BIT_LED);
  delay(1000); // Delay a little bit to improve simulation performance
}