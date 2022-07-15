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
void setup() {
    Serial.begin(9600);
// INPUTS
    //
    // UT INPUTS
    //
    // NOTE: this assumes no daylight savings time
        UTDay = 22;
        UTMonth = 4;
        UTYear = 1980;
        //
        UTHour = 18;
        UTMinute = 36;
        UTSecond = 51.76;
    //
    // GPS CORD INPUT
    //
    // NOTE: assumes E longditudes as positive and W longditudes as negative 
        geoLongDeg = -64; // eventually will need ported to format DDDMM.MMMM
        geoLatDeg = 52;
    //
    // CELESTIAL OBJECT INPUT
    //
    // NOTE: assume each parameter entered individually - this might have to change
        RAHour = 18;
        RAMin = 32;
        RASec = 21;
        //
        DECHour = 23;
        DECMin = 13;
        DECSec = 10;
// Step 1: Convert right ascension to hour angle
    // subtask 1: UT and Greenwich (Julian Calender Date)
        UT = UTHour + UTMinute / 60 + UTSecond / 3600;
        j0 = 367 * UTYear - trunc((7 * (UTYear + trunc((UTMonth + 9) / 12))) / 4) + trunc((275 * UTMonth) / 9) + UTDay + 1721013.5;
        jD = j0 + UT / 24;
        Serial.print("UT [hour decimal] ");
        Serial.println(String(UT));
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
// need to look into variable renaming the units got really messy and need refavtored beacuse I actually know wtf is going on now RA and DEC are measured in diff units that are actually the ~same~
void loop() {
}