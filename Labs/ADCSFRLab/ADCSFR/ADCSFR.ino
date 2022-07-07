// C++ code
//
#include <LiquidCrystal.h>

int seconds = 0;

LiquidCrystal lcd_1(13, 12, 11, 10, 9, 8);

void setup()
{
  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  
  // setup ADC
  // need to enable ADC 
  // set the voltage reference -> AVCC w/ external capacitor at AREF pin
  
  ADCSRA |= _BV(ADEN); // needs to be 1 to enable 
  // for the desired voltage reference, REFS1 is 0 and REFS0 is 1
  ADMUX &= ~_BV(REFS1);
  ADMUX |= _BV(REFS0);
}

void loop()
{
  // set the cursor to column 0, line 1
  lcd_1.setCursor(0, 0);
  lcd_1.print("ADC 0 = ");
  lcd_1.print(getADC0()); // CHANGE TO 15
  lcd_1.print("   ");
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("ADC 3 = ");
  lcd_1.print(getADC15());
  lcd_1.print("   ");
  
  
  
  
  //delay(100);
}

uint16_t getADC0(){
  // select pin using MUX 3 to MUX 1
  ADCSRB &= ~_BV(MUX5);
  ADMUX &= ~_BV(MUX4);
  ADMUX &= ~_BV(MUX3);
  ADMUX &= ~_BV(MUX2);
  ADMUX &= ~_BV(MUX1);
  ADMUX &= ~_BV(MUX0);
  //Start conversion by setting bit
  ADCSRA |= _BV(ADSC);
  // do nothing until conversion is done 
       //while bit is clear ... do no stuff 
  while (bit_is_set(ADCSRA, ADSC)){
    
  }
  //when done return result 
       // goes into 2 registers ADCL ADCH
       // return ADC is better 
  
  return ADC;
}

uint16_t getADC15(){
  
  //CHANGE TO 15
  ADCSRB |= _BV(MUX5);
  ADMUX &= ~_BV(MUX4);
  ADMUX &= ~_BV(MUX3);
  ADMUX |= _BV(MUX2);
  ADMUX |= _BV(MUX1);
  ADMUX |= _BV(MUX0);

  
  ADCSRA |= _BV(ADSC);
  
  while (bit_is_set(ADCSRA, ADSC)){
    
  }
  
  // same but with different MUX 
  return ADC;
}