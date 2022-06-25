// C++ code
//
//uno
/*
#define REG_DDR_PB_PRINT DDRD
#define REG_PORT_PB_PRINT PORTD
#define REG_PIN_PB_PRINT PIND
#define BIT_PB_PRINT 3

#define REG_DDR_PB_COUNT DDRD
#define REG_PORT_PB_COUNT PORTD
#define REG_PIN_PB_COUNT PIND
#define BIT_PB_COUNT 2
*/
//mega

#define REG_DDR_PB_PRINT DDRE
#define REG_PORT_PB_PRINT PORTE
#define REG_PIN_PB_PRINT PINE
#define BIT_PB_PRINT 5

#define REG_DDR_PB_COUNT DDRE
#define REG_PORT_PB_COUNT PORTE
#define REG_PIN_PB_COUNT PINE
#define BIT_PB_COUNT 4

volatile uint16_t isrCount = 0; // or else compiler will try to optomize to 0 and not variable 

void setup()
{
  Serial.begin(9600);
  //pins as inputs
  REG_DDR_PB_PRINT &= ~_BV(BIT_PB_PRINT);
  REG_DDR_PB_COUNT &= ~_BV(BIT_PB_COUNT);
  
  //set pullups
  REG_PORT_PB_PRINT |=_BV(BIT_PB_PRINT);
  REG_PORT_PB_COUNT |=_BV(BIT_PB_COUNT);
  
  //set up interrupt (uno)
  /*
  EIMSK |= _BV(INT0);
  EICRA |= _BV(ISC01); //falling edge setup
  EICRA &= ~_BV(ISC00);//falling edge setup
  */
 
  //set up interrupt (mega)
  EIMSK |= _BV(INT4);
  EICRB |= _BV(ISC41); //falling edge setup
  EICRB &= ~_BV(ISC40);//falling edge setup

}

void loop()
{
  if(bit_is_clear(REG_PIN_PB_PRINT, BIT_PB_PRINT)){
    Serial.println(String("Counter = ") + isrCount);
    delay(500);
  }
  
  //delay(10); // Delay a little bit to improve simulation performance
}

// for uno
/*
ISR(INT0_vect){//weird func (more like macro), in documentation somewhere
  isrCount++;
}
*/

// for mega
ISR(INT4_vect){//weird func (more like macro), in documentation somewhere
delay(20);
if(bit_is_clear(REG_PIN_PB_COUNT, BIT_PB_COUNT)){
    isrCount++;
}
    //delay(50); //debounce fix? -> no
}