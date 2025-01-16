//TAB 6 - Guidance calculations


void guidance() {

  //Read GPS, calculate bearing difference (between current heading and angle to target destination)
  //CHECKL IF THIS READS GPS, SHOULDN'T THAT HAVE ALREADY BEEN DONE IN GPS FUNCTION???
  targetBearing = TinyGPSPlus::courseTo(latitude, longitude, targetLatitude, targetLongitude);
  bearingDifference = targetBearing - courseOverGround;

  //TO INTRODUCE LATER:
  //FIXED CORRECTION (time based) vs. ADAPTIVE CORRECTION (motion based).

  // Adjust calculated bearing difference for the 360-degree wrap-around
  if (bearingDifference > 180) {
      bearingDifference -= 360;
  } else if (bearingDifference < -180) {
      bearingDifference += 360;
  }
    // Determine the turn direction
  if (bearingDifference > 0) {
    correctionDir = Right;
  } else {
    correctionDir = Left;
}

// Check if within desired distance of target. If so, Stop.
destAreaDeg = destAreaMeters*1e-5; // updates desired distance (destination diameter), in case user has changed it.
if (currentOrder != "stop" && abs(latitude - targetLatitude) < destAreaDeg && abs(longitude - targetLongitude) < destAreaDeg){
  
  if (currentOrder == "move"){
    currentOrder = "stop";
    if (millis() > 10000){ //Millis condition to prevent sending message on startup
      dualFeedback("Arrived at destination.");
      delay(1000);
      dualFeedback("Holding position...");
    }
    } else if (currentOrder == "recon"){
      currentOrder = "stop";
      if (millis() > 10000){ //Millis condition to prevent sending message on startup
        dualFeedback("Arrived at destination.");
      }
      delay(2000);
      targetLatitude = startingLatitude;
      targetLongitude = startingLongitude;
      dualFeedback("Returning home.");
      currentOrder = "move";
      }
  }


if (!isCorrectionInProgress){ // If driving straight, update straight timer
        straightTimer = millis(); // Continue tracking how long we've been driving straight
}

    // Check if a correction is needed
    // Conditions include bearing difference, AND straight duration timer
    if (abs(bearingDifference) > degCorrectionNeeded && !isCorrectionInProgress && (straightTimer - straightStart > straightDuration)) {
        isCorrectionInProgress = true; // Start the correction
        corrStart = millis(); //Mark start time for later comparison to corrDuration

        //Below block is for degrees/gyro related calculations
        initialBearingDifference = bearingDifference; // Record the initial degrees bearing difference
        totalTurnedDegrees = 0.0; // Reset total turned degrees
        lastUpdateTime = millis(); // Initialize the time when correction starts

    }
}





