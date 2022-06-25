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

uint16_t pollCount = 0;
uint8_t buttonState = 1;
uint8_t prevButtonState = 1;

void setup()
{
  Serial.begin(9600);
  //pins as inputs
  REG_DDR_PB_PRINT &= ~_BV(BIT_PB_PRINT);
  REG_DDR_PB_COUNT &= ~_BV(BIT_PB_COUNT);
  
  //set pullups
  REG_PORT_PB_PRINT |=_BV(BIT_PB_PRINT);
  REG_PORT_PB_COUNT |=_BV(BIT_PB_COUNT);

}

void loop()
{
  if(bit_is_clear(REG_PIN_PB_PRINT, BIT_PB_PRINT)){
    Serial.println(String("Counter = ") + pollCount);
    delay(500);
  }
  
  //if(bit_is_clear(REG_PIN_PB_COUNT, BIT_PB_COUNT)){
    //pollCount++;
  //}
  
  buttonState = (REG_PIN_PB_COUNT >> BIT_PB_COUNT)& 0x01;
  
  if(buttonState != prevButtonState){
    if (!buttonState){
      pollCount++;
    }
    prevButtonState = buttonState;
    delay(50); //usefull for debounce
  }
  //delay(10); // Delay a little bit to improve simulation performance
}