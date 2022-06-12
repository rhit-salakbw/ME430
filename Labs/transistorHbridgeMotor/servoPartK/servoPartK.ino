// C++ code
//

#include <Servo.h>
int potValue;
int degrees;
Servo myServo; // create servo object 

void setup()
{
  // Inputs
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  // Outputs
  // LEDs
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // transistor/Hbridge
  myServo.attach(9);
  
}

void loop()
{
  // Outputs
  // LEDs
  digitalWrite(4, !digitalRead(2)); // this means opposite/not
  digitalWrite(5, !digitalRead(3)); // this means opposite/not
  analogWrite(6, analogRead(0)/4);  // this to change from 10 bit to 8 bit
  
  // transistor/Hbridge
  if (!digitalRead(2)) {
    myServo.write(0);
  } else if (!digitalRead(3)) {
    myServo.write(180);
  } else {
    potValue = analogRead(0); // reads in from 0 - 1023
    degrees = map(potValue, 0, 1023, 0, 180);
    myServo.write(degrees);
  }
  
 
    
  delay(10);

    
}