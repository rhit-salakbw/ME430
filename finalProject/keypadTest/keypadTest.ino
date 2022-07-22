#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>

//constants for row/col
#define ROWS 4
#define COLS 4

//array to represent keyboard
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//arduino connections
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};


//create keypad object
Keypad inputKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//create lcd
LiquidCrystal menuLCD(42, 44, 46, 48, 50, 52);
void setup(){
    Serial.begin(9600);
}

void loop(){
    char inputKey = inputKeypad.getKey();

    if(inputKey){
        Serial.println(inputKey);
        menuLCD.clear();
        menuLCD.setCursor(0, 1);
        menuLCD.print(inputKey);
    }
}


