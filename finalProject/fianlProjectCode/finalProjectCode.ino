// This will be the framework for the code that will determine how the arm moves
void setup() {
  Serial.begin(9600);
  // Step 1: Convert right ascension to hour angle
    // subtask 1: UT and Greenwich (Julian Calender Date)
        // NOTE: this assumes no daylight savings time
        double UTDay = 22;
        double UTMonth = 4;
        double UTYear = 1980;
        double UTHour = 14;
        double UTMinute = 36;
        double UTSecond = 51.76;
        double UT = UTHour + UTMinute / 60 + UTSecond / 3600;
        double j0 = 367 * UTYear - trunc((7 * (UTYear + trunc((UTMonth + 9) / 12))) / 4) + trunc((275 * UTMonth) / 9) + UTDay + 1721013.5;
        double jD = j0 + UT / 24;
        Serial.println(String(UT));
    // subtask 2: Greenwich sidereal time
        double t0 = (j0 - 2451545) / 36525;
        double thetaG0 = 100.4606184 + 36000.77004 * t0 + .000387933 * pow(t0, 2) - 2.583 * pow(10, -8) * pow(t0, 3);
        Serial.println(String(thetaG0));
        while (0 > thetaG0) {
            thetaG0 = thetaG0 + 360;
        }
        while (thetaG0 > 360) {
            thetaG0 = thetaG0 - 360;
        }
        double thetaG = thetaG0 + 360.98564724 * (UT / 24);
        while (0 > thetaG) {
            thetaG = thetaG + 360;
        }
        while (thetaG > 360) {
            thetaG = thetaG - 360;
        }
        Serial.println(String(thetaG));          // GST in degrres
        Serial.println(String(thetaG * .0067));  // GST in decimal hours
    // subtask 3: local sidereal time
    // subtask 4: express right asension in decimal hours
    // subtask 5: calculate hour angle in hours
    // subtask 6: calculate hour angle in hours minutes and seconds
}

void loop() {
}