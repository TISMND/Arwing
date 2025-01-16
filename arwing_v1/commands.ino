//All of the independent functions for updating variables based on commands
//including but not limited to target coordinates, drive mode, turn duration, turn modifier (rudder angle), straight duration

void updateTargetCoordinates(String data) {
    // Extract and process latitude and longitude from the received data
    int longIndex = data.indexOf("Longitude: ");
    String latString = data.substring(10, longIndex); // Extract latitude string
    String longString = data.substring(longIndex + 11); // Extract longitude string
    // Convert the extracted strings to double and store them in global variables
    targetLatitude = latString.toFloat();  // Changed toFloat() (Used to be .toDouble in 32 Gary but that didn't work here.)
    targetLongitude = longString.toFloat();  // Changed toFloat() (Used to be .toDouble in 32 Gary)
    //Command Response
    HM10Serial.print("Target Latitude: ");
    HM10Serial.println(targetLatitude, 6);
    HM10Serial.print("Target Longitude: ");
    HM10Serial.println(targetLongitude, 6);
    // Record starting position in case recon order is active, and drone needs to return.
    startingLatitude = latitude;
    startingLongitude = longitude;

}

void updateDrivingMode(String mode) {
    // Update the currentMode based on the received data
    currentMode = mode; // Mode string is already trimmed and validated
    HM10Serial.print("Updated Mode: ");
    HM10Serial.println(currentMode);
}

void updateCorrDuration(int cDuration) {
    corrDuration = cDuration;
    HM10Serial.print("Updated Correction Duration: "); HM10Serial.println(corrDuration);
}

void updateTurnAngle(int updatedTurnAngle) {
    turnAngle = updatedTurnAngle; 
    HM10Serial.print("Updated Correction Modifier: "); HM10Serial.println(turnAngle);
}

void updateStraightDur(int sDur) {
    straightDuration = sDur; 
    HM10Serial.print("Updated Straight Duration: "); HM10Serial.println(straightDuration);
}

void updateBaseSpeedCrawl(float speed) {
    baseSpeedCrawl = speed/100; // Update the global variable
    HM10Serial.print("Updated Base Speed Crawl: "); HM10Serial.println(baseSpeedCrawl);
}

void updateBaseSpeedCruise(float speed) {
    baseSpeedCruise = speed/100; // Update the global variable
    HM10Serial.print("Updated Base Speed Cruise: "); HM10Serial.println(baseSpeedCruise);
}

void updateBaseSpeedBurn(float speed) {
    baseSpeedBurn = speed/100; // Update the global variable
    HM10Serial.print("Updated Base Speed Burn: "); HM10Serial.println(baseSpeedBurn);
}

void updateDestArea(int newDia) {
    destAreaMeters = newDia; // Update the global variable
    HM10Serial.print("Updated destination diameter: "); HM10Serial.println(destAreaMeters);
}

void updateDegCorrectionNeeded(float degCorrection) {
    degCorrectionNeeded = degCorrection; // Update the global variable
    HM10Serial.print("Updated Degree Correction Needed: "); HM10Serial.println(degCorrectionNeeded);
}

void updateMidAngle(int angle) {
    midAngle = angle;
    HM10Serial.print("Updated midAngle: "); HM10Serial.println(midAngle);
}

void updateOrderCommand(String orderCommand) {
    currentOrder = orderCommand; 
    HM10Serial.print("Updated Order: "); HM10Serial.println(currentOrder);
}
