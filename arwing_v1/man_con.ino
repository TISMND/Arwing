/*
HARDWARE NOTE, CONTROLLER:
CH 3 - Left stick vertical (throttle) bottom is 1000, top is 2000
CH 4 - Right Stick horizontal (turns) Right is 2000, left is 1000 (RE-DO THE RANGE)
CH  7 - Switch D, far right (toggle auto) Up is 1000, down is 2000

HARDWARE NOTE
FS RECEIVER PIN WIRING:
Top row only, left to right:
1-GND
2-5v
3
4
5
6-FS TX (To board's RX)
*/



void man_con_setup() {
    // Initialize iBus communication on Serial1 with GPIO 19 as RX pin
    ibusSerial.begin(115200);  // Initialize the serial port for iBus communication
    iBusReceiver.begin(ibusSerial);  // Initialize the iBus receiver with the serial port
    

    unsigned long startTime = millis();
    bool ibusDataDetected = false;

    // Check for iBus data with a timeout
    while (millis() - startTime < 200) {  // timeout
        iBusReceiver.loop();  // Check for incoming data
        if (iBusReceiver.readChannel(0) > 0) {  // Check if data is present on channel 0
            ibusDataDetected = true;
            dualFeedbackln("iBus data detected.");
            break;  // Exit the loop once data is detected
        }
    }
    if (!ibusDataDetected) {
        dualFeedbackln("iBus data not detected. Note:");
        dualFeedbackln("Module must initialize by connecting with controller.");
    }
}




void controlCheck(){ //Checks of flysky switdh D is flipped to manual or control mode.
    iBusReceiver.loop();
    switchDValue = iBusReceiver.readChannel(6);   // This is the seventh item.
    //Serial.print(switchDValue);
    if ((switchDValue == 1000 && !autopilot) || (switchDValue == 0 && !autopilot)) {  // Switch D up (autopilot mode)
        autopilot = true;
        manualOverride = false;
       
        if(millis()>8000){ //delay so tone doesn't play on startup
          tone_sat_nav_on();
        }

        dualFeedbackln("Autopilot active.");
    } else if (switchDValue == 2000 && !manualOverride) {  // Switch D down (manual control)
        autopilot = false;
        manualOverride = true;
        if(millis()>8000){ //delay so tone doesn't play on startup
          tone_manual_con_on();
        }
        dualFeedbackln("Manual control enabled.");

    }

}

void manual_control() {
      iBusReceiver.loop(); // Update control inputs based on received iBus data
    
/*
  //Prints receiver values for debugging  
  for (int i = 0; i < 10; i++) {  // Adjust '10' to the maximum channels supported by your receiver
      Serial.print(iBusReceiver.readChannel(i));  // Use iBusReceiver to read the channel values
      Serial.print(" ");
  }
  Serial.println(" ");
*/


  if (iBusReceiver.readChannel(1) > 0) {
      // 0 indexing makes it weird
      throttleRX = iBusReceiver.readChannel(2);  // This is the third item in ibus stream.
      steeringValue = iBusReceiver.readChannel(3);  // This is the fourth item in ibus stream.
      reverse = iBusReceiver.readChannel(4);
      switchDValue = iBusReceiver.readChannel(6);   // This is the seventh item in ibus stream.
  
  }


/*
//Prints specific receiver values for debugging
    Serial.print("Throttle: ");
    Serial.println(throttleRX);
    Serial.print("Steering: ");
    Serial.println(steeringValue);
    Serial.print("Switch D: ");
    Serial.println(switchDValue);
*/



    //FORWARD
    if (reverse == 1000) { //(If switch D is flupped up)
        throttle = map(throttleRX, 1000, 2000, 1500, 2000);
        esc1.writeMicroseconds(throttle);  // Apply the calculated pulse width to the ESC
        esc2.writeMicroseconds(throttle);  // Apply the calculated pulse width to the ESC

        rudderPosition = map(steeringValue, 1000, 2000, 0, 180);  // Map the steering range to 0-180 degrees
        //rudder1.write(rudderPosition);  // Apply the calculated angle to the servo
        rudder2.write(rudderPosition);  // Apply the calculated angle to the servo
    }

    //Reverse
    if (reverse == 2000) {
             // esc.writeMicroseconds(throttle); 
       // delay(500);
        throttle = map(throttleRX, 1000, 2000, 1500, 1000);
        esc1.writeMicroseconds(throttle);  // Apply the calculated pulse width to the ESC
        esc2.writeMicroseconds(throttle);  // Apply the calculated pulse width to the ESC
        rudderPosition = map(steeringValue, 1000, 2000, 0, 180);  // Map the steering range to 0-180 degrees
        //rudder1.write(rudderPosition);  // Apply the calculated angle to the servo
        rudder2.write(rudderPosition);  // Apply the calculated angle to the servo
    }
}

