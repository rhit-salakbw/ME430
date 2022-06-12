// C++ code
#include <Stepper.h>
// LEDS DONT WORK ANYMORE BECAUSE SET FOR ACTUAL BOARD

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// 32 steps X 64:1 gearhead
//const int stepsPerRevolution = 60;  // JUST FOR SIMULATION -- NOT CORRECT FOR REAL MOTOR

// for your motor


// initialize the stepper library on pins 8 through 11:

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // order is important here 


void setup() {

  // set the speed at 5 rpm:

  myStepper.setSpeed(5);

  // initialize the serial port:

  //Serial.begin(9600);

}


void loop() {

  // step one revolution  in one direction:

  //Serial.println("clockwise");

  myStepper.step(stepsPerRevolution);

  delay(500);


  // step one revolution in the other direction:

  //Serial.println("counterclockwise");

  myStepper.step(-stepsPerRevolution);

  delay(500);

}