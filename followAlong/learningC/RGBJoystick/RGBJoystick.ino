// C++ code
//
int rPot;
int bPot;

void setup()
{
  Serial.begin(9600); //com speed. bits per second
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(2, INPUT_PULLUP);
  
}

void loop()
{
/*   red();
  delay(1000);
  green();
  delay(1000);
  off();
  delay(1000); */

  rPot = analogRead(0);
  Serial.print(" R=");
  Serial.print(rPot);
  
  bPot = analogRead(1);
  Serial.print(" B=");
  Serial.println(bPot);

  if (digitalRead(2)) {
    rgb(rPot/4, 0, bPot/4);
  } else {
    off();
  }
  
  
  delay(500);


}

void rgb(int r, int g, int b) {
  // parameters should be 0 to 255
  analogWrite(9, r);
  analogWrite(10, g);
  analogWrite(11, b);
  
}

void red() {
  analogWrite(9, 255);
  analogWrite(10, 0);
  analogWrite(11, 0);
}

void green() {
  analogWrite(9, 0);
  analogWrite(10, 255);
  analogWrite(11, 0);
}

void off() {
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
}