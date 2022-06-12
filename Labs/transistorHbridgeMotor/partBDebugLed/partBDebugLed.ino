// C++ code
//
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
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);  
}

void loop()
{
  // Outputs
  // LEDs
  digitalWrite(4, !digitalRead(2)); // this means opposite/not
  digitalWrite(5, !digitalRead(3)); // this means opposite/not
  analogWrite(6, analogRead(0)/4);  // this to change from 10 bit to 8 bit
  
  // transistor/Hbridge
  digitalWrite(12, !digitalRead(2)); // this means opposite/not
  digitalWrite(13, !digitalRead(3)); // this means opposite/not
  analogWrite(11, analogRead(0)/4);  // this to change from 10 bit to 8 bit
    
  delay(10);

    
}