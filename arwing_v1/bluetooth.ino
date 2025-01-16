//Bluetooth HM10

// HARDWARE NOTE ABOUT SETTING BAUD RATE ON HM10:
  // HM10 default baud is 9600. Check modules rate with AT+BAUD?   (BUT BELOW SAYS OTHERWISE??)
  // Instead of displaying the full number, they're broken down into settings 0, 1, 2, 3, 4 (maybe more)
  // 0 is 9600, 4 is 115200
  //Use AT+BAUD4 to set HM10 rate to 115200. Reboot module for this to take effect. Or try AT+RESET


void bluetooth_setup() {
    // Initialize HM10Serial on other available pins
    //HM10 isn't responding to Teensy AT Commands, so no start up check available until that's fixed.
    dualFeedbackln("Starting HM10 Serial.");
    }





void bluetoothRX() {

/*    
// Echo messages from HM10 to SatCom with newline characters
if (HM10Serial.available()) {
    String hm10Message = HM10Serial.readStringUntil('\n');
    satComSerial.print(hm10Message); // Add newline for RockBLOCK
    HM10Serial.print("Sent to RockBlock: " + hm10Message);
    delay(1000);
}
// Echo messages from SatCom to HM10
if (satComSerial.available()) {
    String satcomMessage = satComSerial.readStringUntil('\n');
    HM10Serial.print("Received from RockBlock: " + satcomMessage);
}
*/


    if (HM10Serial.available()) {
        String receivedData = HM10Serial.readStringUntil('\n'); // Read the incoming data until newline
        receivedData.trim(); // Remove any whitespace or newline characters
        HM10Serial.print("Received: "); // Print the complete message
        HM10Serial.println(receivedData);

        // Determine the type of data received and call the appropriate function
        if (receivedData.startsWith("Latitude: ") && receivedData.indexOf("Longitude: ") != -1) {
            updateTargetCoordinates(receivedData);
        } else if (receivedData.startsWith("dm ")) {
            // Extract the drive mode command after the "DM " prefix
            String driveModeCommand = receivedData.substring(3); // Skip the first three characters ("DM ")
            updateDrivingMode(driveModeCommand);
        } else if (receivedData.startsWith("cd ")) {
            int updatedCorrDur = receivedData.substring(3).toInt(); // Convert substring to int
            updateCorrDuration(updatedCorrDur);
        } else if (receivedData.startsWith("ta ")) {
            int updatedTurnAngle = receivedData.substring(3).toInt(); // Convert substring to float
            updateTurnAngle(updatedTurnAngle);
        } else if (receivedData.startsWith("sd ")) {
            int updatedStraightDur = receivedData.substring(5).toInt(); // Convert substring to int
            updateStraightDur(updatedStraightDur);
        } else if (receivedData.startsWith("cram ")) {
            float updatedBaseSpeedCrawl = receivedData.substring(5).toFloat(); // Convert substring to float
            updateBaseSpeedCrawl(updatedBaseSpeedCrawl);
        } else if (receivedData.startsWith("crum ")) {
            float updatedBaseSpeedCruise = receivedData.substring(5).toFloat(); // Convert substring to float
            updateBaseSpeedCruise(updatedBaseSpeedCruise);
        } else if (receivedData.startsWith("burm ")) {
            float updatedBaseSpeedBurn = receivedData.substring(4).toFloat(); // Convert substring to float
            updateBaseSpeedBurn(updatedBaseSpeedBurn);
        } else if (receivedData.startsWith("dam ")) {
            int updatedDestArea = receivedData.substring(4).toInt(); // Convert substring to int
            updateDestArea(updatedDestArea);
        } else if (receivedData.startsWith("dcn ")) {
            float updatedDegCorrectionNeeded = receivedData.substring(4).toFloat(); // Convert substring to float
            updateDegCorrectionNeeded(updatedDegCorrectionNeeded);
        } else if (receivedData.startsWith("cents ")) {
            int angle = receivedData.substring(6).toInt();  // Convert substring to int
            updateMidAngle(angle);  // Trigger the function to update the global variable
        } else if (receivedData.startsWith("order ")) {
            String orderCommand = receivedData.substring(6); // Skip the first six characters ("order ")
            updateOrderCommand(orderCommand); // Assuming you have a function to handle the order command
        } else if (receivedData.startsWith("cmds?")) {
            showCommands();
        } else if (receivedData.startsWith("output?")) {
            showOutputCommands();
        } else if (receivedData.startsWith("stats?")) {
            showStats();
        } else if (receivedData.startsWith("data?")) {
            showDataCommands();
        } else if (receivedData.startsWith("data ")) { //sub tree for commands involving data recording on Teensy 4.1 SD card.
            String dataCommand = receivedData.substring(5); // Extract the data command after "data "
            if (dataCommand.startsWith("new file ")) {
                String newFileName = dataCommand.substring(9); // Extract the new file name
                fileName = newFileName + ".txt"; // Add the ".txt" extension
                newFile = true;
                HM10Serial.print("New file set: ");
                HM10Serial.println(fileName);
            } else if (dataCommand == "record") {
                recording = true;
                HM10Serial.println("Data recording started.");
            } else if (dataCommand == "stop") {
                recording = false;
                HM10Serial.println("Data recording stopped.");
            } else {
                HM10Serial.println("Unrecognized data command");
            }
        } else if (receivedData.startsWith("output ")) {
            String outputCommand = receivedData.substring(7); // Extract the output command after "output "
             if (outputCommand == "current") {
                outputCurrent = !outputCurrent; // Toggle the outputCurrent boolean
            } else if (outputCommand == "temp") {
                outputTemp = !outputTemp; // Toggle the outputTemp boolean
            } else if (outputCommand == "bat") {
                outputBat = !outputBat; // Toggle the outputBat boolean
            } else if (outputCommand == "speed") {
                outputSpeed = !outputSpeed; // Toggle the outputSpeed boolean
            } else if (outputCommand == "all") {
                outputCurrent = true;
                outputTemp = true;
                outputBat = true;
                outputSpeed = true;
                HM10Serial.println("All outputs enabled");
            } else if (outputCommand == "off") {
                outputCurrent = false;
                outputTemp = false;
                outputBat = false;
                outputSpeed = false;
                HM10Serial.println("All outputs disabled");
            } else {
                HM10Serial.println("Unrecognized output command");
            }    
        } else if (receivedData.startsWith("OK+LOST")) {
            tone_bt_disconnected();
            Serial.println("HM10 disconnected.");
            HM10Serial.println("HM10 disconnected");
        
        }else if (receivedData.startsWith("OK+CONN")) {
            tone_bt_connected();
            Serial.println("HM10 connection successful.");
            HM10Serial.println("Connection successful. Welcome.");
            bt_main_menu();
        } else { // Nothing recognized. Error.
          if(millis()>5000){ // Delay so error tone doesn't play on startup
            tone_unknown_msg();
          }
            Serial.println("Unrecognized data format" + receivedData);
            HM10Serial.println("Unrecognized data format: " + receivedData);
            bt_main_menu();
        }
        
    }



  if (millis() - previousOutputMillis >= 1000) {
      previousOutputMillis = millis();
     if (outputCurrent) {
            HM10Serial.print("Draw: "); HM10Serial.print(currentDraw); HM10Serial.println("A"); 
        }
        if (outputTemp) {
            HM10Serial.print("T1: "); HM10Serial.print(temperature1); HM10Serial.print("C");
            HM10Serial.print("    ");
            HM10Serial.print("T2: "); HM10Serial.print(temperature2); HM10Serial.println("C");
        }
        if (outputBat) {
            HM10Serial.print("BAT V: "); HM10Serial.print(batteryVoltage); HM10Serial.print("V");
        }
        if (outputSpeed) {
            HM10Serial.print("Spd: "); HM10Serial.print(speedKnots); HM10Serial.println(" knots"); 
        }
        if (outputCurrent || outputTemp || outputBat || outputSpeed){
          HM10Serial.println(" ");
  }

}


}

    void bt_main_menu(){
          HM10Serial.println("Enter 'cmds?' for help.");
          HM10Serial.println("Enter 'data?' for SD Card data recording help.");
          HM10Serial.println("Enter 'output?' for bluetooth output options.");
          HM10Serial.println("Enter 'stats?' for variable list.");
    }






void showCommands(){
    HM10Serial.println(" ");
    HM10Serial.println("COMMANDS LIST: ");
    HM10Serial.println(" ");
    HM10Serial.println("dm = drive mode ");
    HM10Serial.println("(neutral, crawl, cruise, or burn) ");
    HM10Serial.println(" ");
    HM10Serial.println("cd = correction duration (in millis) ");
    HM10Serial.println(" ");
    HM10Serial.println("ta = turn angle (int 0-90, safe 20-70) ");
    HM10Serial.println(" ");
    HM10Serial.println("sd = Straight duration (in millis) ");
    HM10Serial.println(" ");
    delay(75);
    HM10Serial.println("cram = Crawl Modifier (0-100) ");
    HM10Serial.println(" ");
    HM10Serial.println("crum = Cruise Modifier (0-100)  ");
    HM10Serial.println(" ");
    HM10Serial.println("burm = Burn Modifier (0-100)  ");
    HM10Serial.println(" ");
    delay(75);
    HM10Serial.println("dam = Diameter of targ dest. (in meters) ");
    HM10Serial.println(" ");
    HM10Serial.println("dcn = Max deg off-course for correction ");
    HM10Serial.println("(Degrees, Integer or Decimal)");
    HM10Serial.println(" ");
    delay(75);
    HM10Serial.println("cents = Center Servo Angle (int 0-180) ");
    HM10Serial.println(" ");
    HM10Serial.println("order = stop, move, or recon ");
    HM10Serial.println(" ");
    HM10Serial.println(" ");
}



void showOutputCommands() {
    HM10Serial.println(" ");
    HM10Serial.println("OUTPUT COMMANDS LIST: ");
    HM10Serial.println(" ");
    HM10Serial.println("output current = Toggle current output ");
    HM10Serial.println(" ");
    HM10Serial.println("output temp = Toggle temperature output ");
    HM10Serial.println(" ");
    HM10Serial.println("output bat = Toggle battery output ");
    HM10Serial.println(" ");
    HM10Serial.println("output speed = Toggle speed output ");
    HM10Serial.println(" ");
    HM10Serial.println("output all = Enable all outputs ");
    HM10Serial.println(" ");
    HM10Serial.println("output off = Disable all outputs ");
    HM10Serial.println(" ");
}



void showStats() {
    HM10Serial.println(" ");
    HM10Serial.println("CURRENT STATUS: ");
    HM10Serial.println(" ");
    HM10Serial.print("Current Order: "); HM10Serial.println(currentOrder);
    HM10Serial.println(" ");
    HM10Serial.print("Current Drive Mode: "); HM10Serial.println(currentMode);
    HM10Serial.println(" ");
    HM10Serial.print("Correction Duration: "); HM10Serial.print(corrDuration); HM10Serial.println("ms ");
    HM10Serial.println(" ");
    delay(50);
    HM10Serial.print("Turn Angle: "); HM10Serial.print(turnAngle); HM10Serial.println("deg");
    HM10Serial.println(" ");
    HM10Serial.print("Servo Center Angle: "); HM10Serial.print(midAngle); HM10Serial.println("deg ");
    HM10Serial.println(" ");
    HM10Serial.print("Degree Correction Needed: "); HM10Serial.print(degCorrectionNeeded); HM10Serial.println("deg");
    HM10Serial.println(" ");
    HM10Serial.print("Straight Duration: "); HM10Serial.print(straightDuration); HM10Serial.println("ms ");
    HM10Serial.println(" ");
    HM10Serial.print("Crawl Spd. Modifier: "); HM10Serial.print(static_cast<int>(baseSpeedCrawl * 100)); HM10Serial.println("%");
    HM10Serial.println(" ");
    delay(50);
    HM10Serial.print("Cruise Spd. Modifier: "); HM10Serial.print(static_cast<int>(baseSpeedCruise * 100)); HM10Serial.println("%");
    HM10Serial.println(" ");
    HM10Serial.print("Burn Spd. Modifier: "); HM10Serial.print(static_cast<int>(baseSpeedBurn * 100)); HM10Serial.println("%");
    HM10Serial.println(" ");
    HM10Serial.print("Destination Area Diameter: "); HM10Serial.print(destAreaMeters); HM10Serial.println("m");
    delay(50);
    HM10Serial.println(" ");
    HM10Serial.print("Present Latitude: "); HM10Serial.println(latitude,12);
    HM10Serial.print("Present Longitude: "); HM10Serial.println(longitude,12);
    HM10Serial.println(" ");
    HM10Serial.print("Target Latitude: "); HM10Serial.println(targetLatitude,12);
    HM10Serial.print("Target Longitude: "); HM10Serial.println(targetLongitude,12);
    HM10Serial.println(" ");
}

void showDataCommands() {
    HM10Serial.println(" ");
    HM10Serial.println("DATA COMMANDS LIST: ");
    HM10Serial.println(" ");
    HM10Serial.println("data new file [filename] -- no brackets");
    HM10Serial.println(" ");
    HM10Serial.println("data record = Start recording data");
    HM10Serial.println(" ");
    HM10Serial.println("data stop = Stop recording data");
    HM10Serial.println(" ");
}



void dualFeedbackln(String message) {
    Serial.println(message);
    HM10Serial.println(message);
}

void dualFeedback(String message) {
    Serial.print(message);
    HM10Serial.print(message);
}






