
void gps_setup() {
    // Initialize hardware serial communication with the GPS module on Serial2
    GPSSerial.begin(9600, SERIAL_8N1);

    unsigned long startTime = millis();
    bool nmeaDataDetected = false;

    // Check for NMEA data on startup, with a 1-second timeout
    while (millis() - startTime < 1000) {  // 1-second timeout
        if (GPSSerial.available() > 0) {
            char c = GPSSerial.read();
            
            if (c == '$') {  // '$' indicates start of NMEA sentence
                nmeaDataDetected = true;
                dualFeedbackln("NMEA GPS data stream detected.");

                break;  // Exit loop once data is detected
            }
        }
    }

    if (!nmeaDataDetected) {
        dualFeedbackln("NMEA data not detected.");
    }
}


void gps_read() {
    while (GPSSerial.available() > 0) {
        char c = GPSSerial.read();
        //Serial.print(c); // Print the raw NMEA data character
        if (gps.encode(c)) {
            // Update latitude and longitude based on reading
            if (gps.location.isUpdated()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
            }
            // Update Course Over Ground
            if (gps.course.isUpdated()) {
                courseOverGround = gps.course.deg();
            }
            // Update Speed
            if (gps.speed.isUpdated()) {
                speedKnots = gps.speed.knots();
            }
            

        }
    }
}
