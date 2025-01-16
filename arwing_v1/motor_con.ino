//---------------------------------------------------------------
//----------------------SERVO (RUDDER) CONTROL-------------------
//---------------------------------------------------------------

// Infrastructure in place for dual rudders.
// But for now, control set up for single-rudder configuration.

void servo_setup() {
    // rudder1.attach(rudder1Pin, 1000, 2000);  // Attaches the servo on pin 27 to the servo object
                                           // and sets min/max pulse width to 1000/2000 microseconds
    rudder2.attach(rudder2Pin, 1000, 2000);                                          
}

void servo_control() {
    // Using indirect approach with "rudderPosition" var because need a global variable for data recording.
if (currentOrder != "stop") {
    if (isCorrectionInProgress) {
        if (correctionDir == Right) { //Right Turn
            rudderPosition = (midAngle + turnAngle);
        //    rudder1.write(rudderPosition);
            rudder2.write(rudderPosition);
        } else { // -------Left turn
            rudderPosition = (midAngle - turnAngle);
          //  rudder1.write(rudderPosition);
            rudder2.write(rudderPosition);
        }
    } else {
        rudderPosition = midAngle;
       // rudder1.write(rudderPosition);
        rudder2.write(rudderPosition);
    }
}
}





//----------------------------------------------------------------
//----------------------BRUSHLESS ESC CONTROL---------------------
//----------------------------------------------------------------

// Able to control twin motors (optional).
// No differential thrust, yet.


void drive_setup() {
    esc1.attach(esc1Pin);    // Attach the first ESC signal to its pin
    esc2.attach(esc2Pin);    // Attach the second ESC signal to its pin
    esc1.writeMicroseconds(neutralPulseWidth); // Send neutral signal to arm the first ESC
    esc2.writeMicroseconds(neutralPulseWidth); // Send neutral signal to arm the second ESC
    dualFeedbackln("Enabling ESCs with neutral signal...");
    delay(2000);            // Delay for the ESCs to recognize the neutral signal (Otherwise it might lock-out)
}




void drive_control() {

if (currentOrder == "move" || currentOrder == "recon"){
    if (currentMode == "neutral") {
        throttle = neutralPulseWidth;
    } else if (currentMode == "crawl") {
        throttle = neutralPulseWidth + (maxPulseWidth - neutralPulseWidth) * baseSpeedCrawl;
    } else if (currentMode == "cruise") {
        throttle = neutralPulseWidth + (maxPulseWidth - neutralPulseWidth) * baseSpeedCruise;
    } else if (currentMode == "burn") {
        throttle = neutralPulseWidth + (maxPulseWidth - neutralPulseWidth) * baseSpeedBurn;
    } else {
        throttle = neutralPulseWidth;  // Default to neutral if mode is unrecognized
    }
}
    esc1.writeMicroseconds(throttle); // Apply the calculated pulse width to the ESC
    esc2.writeMicroseconds(throttle); // Apply the calculated pulse width to the ESC
}


