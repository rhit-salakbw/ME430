// This will be the framework for the code that will determine how the arm moves
// Variable Definition
    double UTDay;
    double UTMonth;
    double UTYear;
    double UTHour;
    double UTMinute;
    double UTSecond;
    double UT;
    double j0;
    double jD;
    double t0;
    double thetaG0;
    double thetaGDeg;
    double thetaGHA;
    double geoLongDeg;
    double thetaSDeg;
    double thetaSHA;
    double RAHour;
    double RAMin;
    double RASec;
    double RAHA;
    double HourAngleDeg;
    double HourAngleHA;
    double DECHour;
    double DECMin;
    double DECSec;
    double DECDeg;
    double geoLatDeg;
    double sinAlt;
    double alt;
    double cosAz;
    double azPrime;
    double sinHA;
    double az;

    int currState = 1;
    int prevState = 0;

//constant definiton
#define PREVBUTTON 22
#define NEXTBUTTON 24
#define CWBUTTON 26
#define CCWBUTTON 28

void setup() {
    //open serial
        Serial.begin(9600);
    //define 4 buttons for menu navigation 
        //back
        pinMode(PREVBUTTON, INPUT_PULLUP);
        //next
        pinMode(NEXTBUTTON, INPUT_PULLUP);
        //cw - clockwise
        pinMode(CWBUTTON, INPUT_PULLUP);
        //ccw - counter clockwise 
        pinMode(CCWBUTTON, INPUT_PULLUP);
}

void loop() {
    if (currState != prevState){
        Serial.print("state = ");
        Serial.println(currState);
    }       
    stateMachine();

}

void stateMachine(){
    prevState = currState;
    switch (currState) {
        case 1: //start screen
            if (digitalRead(NEXTBUTTON) == 0){
                currState = 2;
                delay(500);
            }
        break;

        case 2: //observatory define
            if (digitalRead(PREVBUTTON) == 0){ //go back to start screen
                currState = 1;
                delay(500);
            } else if (digitalRead(CWBUTTON) == 0){ //go to automatic calibration
                currState = 3;
                delay(500);
            } else if (digitalRead(CCWBUTTON) == 0){ //go to manual calibration step 1 
                currState = 4;
                delay(500);
            }
        break;
        
        case 3: //automatic observatory
            //this will go automatically once sequence is complete
            if (digitalRead(PREVBUTTON) == 0){ //go back to observatory define
                currState = 2;
                delay(500);
            }
        break;

        case 4: //enter time
            if (digitalRead(PREVBUTTON) == 0){ //go back to observatory define
                currState = 2;
                delay(500);
            } else if (digitalRead(NEXTBUTTON) == 0){ //go to longditude entry
                currState = 5;
                delay(500);   
            } 
        break;

        case 5: //enter longditude 
            if (digitalRead(PREVBUTTON) == 0){ //go back to time entry
                currState = 4;
                delay(500);
            } else if (digitalRead(NEXTBUTTON) == 0){ //go to latitude entry
                currState = 6;
                delay(500);   
            }
        break;

        case 6: //enter latitude
            if (digitalRead(PREVBUTTON) == 0){ //go back to longditude entry
                currState = 5;
                delay(500);
            } else if (digitalRead(NEXTBUTTON) == 0){ //go to alignment sequence
                currState = 7;
                delay(500);  
            } 
        break;

        case 7: //alignment sequence 
        break;


    }
}
void calcAltAz() {
// INPUTS
    //
    // UT INPUTS
    //
    // NOTE: this assumes no daylight savings time
        UTDay = 19;
        UTMonth = 7;
        UTYear = 2022;
        //
        UTHour = 3;
        UTMinute = 6;
        UTSecond = 00;
    //
    // GPS CORD INPUT
    //
    // NOTE: assumes E longditudes as positive and W longditudes as negative 
        geoLongDeg = 94; // eventually will need ported to format DDDMM.MMMM
        geoLatDeg = 39;
    //
    // CELESTIAL OBJECT INPUT
    //
    // NOTE: assume each parameter entered individually - this might have to change
        RAHour = 6;
        RAMin = 00;
        RASec = 00;
        //
        DECHour = -16;
        DECMin = 00;
        DECSec = 00;
// Step 1: Convert right ascension to hour angle
    // subtask 1: UT and Greenwich (Julian Calender Date)
        UT = UTHour + UTMinute / 60 + UTSecond / 3600;
        j0 = 367 * UTYear - trunc((7 * (UTYear + trunc((UTMonth + 9) / 12))) / 4) + trunc((275 * UTMonth) / 9) + UTDay + 1721013.5;
        jD = j0 + UT / 24;
        Serial.print("UT [hour decimal] ");
        Serial.println(String(UT));
        Serial.print("Julian Date [hour decimal] ");
        Serial.println(String(jD));
    // subtask 2: Greenwich sidereal time
        t0 = (j0 - 2451545) / 36525;
        thetaG0 = 100.4606184 + 36000.77004 * t0 + .000387933 * pow(t0, 2) - 2.583 * pow(10, -8) * pow(t0, 3);
        //Serial.println(String(thetaG0));
        while (0 > thetaG0) {
            thetaG0 = thetaG0 + 360;
        }
        while (thetaG0 > 360) {
            thetaG0 = thetaG0 - 360;
        }
        thetaGDeg = thetaG0 + 360.98564724 * (UT / 24);
        
        while (0 > thetaGDeg) {
            thetaGDeg = thetaGDeg + 360;
        }
        while (thetaGDeg > 360) {
            thetaGDeg = thetaGDeg - 360;
        }   
        Serial.print("Greenwich Sidereal Time [degrees] ");
        Serial.println(String(thetaGDeg));          // GST in degrres
        thetaGHA = (thetaGDeg * 24)/360;
        Serial.print("Greenwich Sidereal Time [hour decimal] ");
        Serial.println(String(thetaGHA));  // GST to hour angle 
    // subtask 3: local sidereal time
        thetaSDeg = thetaGDeg + geoLongDeg;
        while (0 > thetaSDeg) {
            thetaSDeg = thetaSDeg + 360;
        }
        while (thetaSDeg > 360) {
            thetaSDeg = thetaSDeg - 360;
        }    
        Serial.print("Local Sidereal Time [degrees] ");
        Serial.println(String(thetaSDeg));
        Serial.print("Local Sidereal Time [hour decimal] ");
        thetaSHA = (thetaSDeg*24)/360;
        Serial.println(String(thetaSHA));
    // subtask 4: express right asension in decimal hours
        RAHA = RAHour + RAMin / 60 + RASec / 3600;
        Serial.print("Right Ascension [hour decimal] ");
        Serial.println(String(RAHA));
    // subtask 5: calculate hour angle 
        HourAngleDeg = thetaSDeg - (RAHA*360)/24;
        while (0 > HourAngleDeg) {
            HourAngleDeg = HourAngleDeg + 360;
        }
        while (HourAngleDeg > 360) {
            HourAngleDeg = HourAngleDeg - 360;
        }
        Serial.print("Hour Angle [degrees] ");  
        Serial.println(String(HourAngleDeg));    
        HourAngleHA = (HourAngleDeg * 24)/360;     
        Serial.print("Hour Angle [hour decimal] ");  
        Serial.println(String(HourAngleHA));
// Step 2: Equitorial to Horizon Conversion
    // subtask 1: swap Ha values to example to check code - comment out later
        //HourAngleHA = 5.862;
        //HourAngleDeg = 87.97;
    // subtask 2: convert dec into degrees
        DECDeg = DECHour + DECMin / 60 + DECSec / 3600;
        Serial.print("Declination Angle [degrees] ");  
        Serial.println(String(DECDeg)); 
    // subtask 3: calculate sin(a) a = altitude 
        sinAlt = sin(DECDeg*(PI/180))*sin(geoLatDeg*(PI/180)) + cos(DECDeg*(PI/180))*cos(geoLatDeg*(PI/180))*cos(HourAngleDeg*(PI/180));
        Serial.print("sin(a) [] ");  
        Serial.println(String(sinAlt)); 
    // subtask 4: take inverse of sin(a) 
        alt = asin(sinAlt)*(180/PI);
        Serial.print("altitude [degrees] ");  
        Serial.println(String(alt)); 
    // subtask 5: calculate cos(A) A = azimuth
        cosAz = (sin(DECDeg*(PI/180)) - (sin(geoLatDeg*(PI/180))*sin(alt*(PI/180))))/(cos(geoLatDeg*(PI/180))*cos(alt*(PI/180)));
        Serial.print("cos(A) [] ");  
        Serial.println(String(cosAz)); 
    // subtask 6: take inverse and calculate A' -> modify A if needed
        azPrime = acos(cosAz)*(180/PI);
        Serial.print("azimuth prime [degrees] ");  
        Serial.println(String(azPrime)); 
        sinHA = sin(HourAngleDeg*(PI/180));
        Serial.print("sin(HourAngle) [degrees] ");  
        Serial.println(String(sinHA)); 
        if (sinHA < 0) {
            az = azPrime;
        } else {
            az = 360 - azPrime;
        }
        Serial.print("azimuth [degrees] ");  
        Serial.println(String(az)); 
    // report values


}
// need to look into variable renaming the units got really messy and need refavtored beacuse I actually know wtf is going on now 
//RA and DEC are measured in diff units that are actually the ~same~