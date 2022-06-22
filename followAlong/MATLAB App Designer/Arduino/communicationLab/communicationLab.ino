#include <Servo.h>

#define PIN_PUSHBUTTON 2
#define PIN_LED 3
#define PIN_JOYSTICK_X 0
#define PIN_JOYSTICK_Y 1
#define PIN_SERVO 9

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
Servo myServo; // create servo object 

int joystickX;
int lastSentJoystickX = 0;

int joystickY;
int lastSentJoystickY = 0;

#define JOYSTICK_THRESHOLD 15

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUSHBUTTON, INPUT_PULLUP);
  myServo.attach(PIN_SERVO);  
  
}

void loop() { //start loop
  // print the string when a newline arrives:
  
  if (!digitalRead(2)) {//start if
    Serial.println("B");
    delay(500);
  } // end if
  // joystick stuff 
  joystickX = analogRead(PIN_JOYSTICK_X);
  
  if (abs(joystickX - lastSentJoystickX) > JOYSTICK_THRESHOLD) {// start if
    Serial.println(String("X ") + joystickX);
    lastSentJoystickX = joystickX;
  }// end if

  joystickY = analogRead(PIN_JOYSTICK_Y);
  
  if (abs(joystickY - lastSentJoystickY) > JOYSTICK_THRESHOLD) {// start if
    Serial.println(String("Y ") + joystickY);
    lastSentJoystickY = joystickY;
  }// end if

      
  if (stringComplete) {// start big if
    Serial.println(inputString); //optional
    inputString.trim();
    
    // do stuff with string 
    
    if (inputString.equalsIgnoreCase("LED ON")){ //start if
      digitalWrite(PIN_LED, HIGH);   
    }// end if
    
    if (inputString.equalsIgnoreCase("LED OFF")){//start if
      digitalWrite(PIN_LED, LOW);
    }// end if
    
     if (inputString.startsWith("SERVO ")){//start if
		int degrees = inputString.substring(6).toInt();
        myServo.write(degrees);       
     }// end if
    
 
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }//end big if
  //delay(10);
}// end loop

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') { // called line feed. ASCII value 10
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
