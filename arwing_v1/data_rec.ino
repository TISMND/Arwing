// Control of the Teensy 4.1's onboard SD card slot.
// User must activate recording by message, see bluetooth.

void sd_card_setup() {
    Serial.begin(115200);
    if (!SD.begin(chipSelect)) {
        dualFeedback("Card failed, or not present");
    }
}

void new_data_file(){
    // Open the file for writing
    dataFile = SD.open(fileName.c_str(), FILE_WRITE);

    // If the file opened okay, write the header
    if (dataFile) {
        dataFile.println("Time(ms),CurrentDraw(A),Throttle(%),Speed(knots),Temp1(C),Temp2(C),Rudder Position(deg),BatteryVoltage(V)");
        dataFile.close();
    } else {
        Serial.print("Error opening file: ");
        Serial.println(fileName);
    }
    newFile = false;
}


void data_record() {
    unsigned long timeNow = millis();

    // Map throttle to throttle percentage
    int throttlePercent = map(throttle, 1000, 2000, 0, 100);

    // Open the file for appending
    dataFile = SD.open(fileName.c_str(), FILE_WRITE);


    // If the file opened okay, write the data
    if (dataFile) {
        dataFile.print(timeNow);
        dataFile.print(",");
        dataFile.print(currentDraw);
        dataFile.print(",");
        dataFile.print(throttlePercent);
        dataFile.print(",");
        dataFile.print(speedKnots);
        dataFile.print(",");
//        dataFile.print(therm1);
        dataFile.print(",");
//        dataFile.print(therm2);
        dataFile.print(",");
        dataFile.print(rudderPosition);
        dataFile.print(",");
        dataFile.print(batteryVoltage);
        dataFile.close();
    } else {
        Serial.println("Error opening datalog.");
    }



}

