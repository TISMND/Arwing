#include <Arduino.h>

// Interacts with the RockBLOCK 9603 module

void satcom_setup() {
    satComSerial.begin(19200); 
}


// Moves through stages with each main loop, depending on feedback from rockblock.
void transmit_sbd() { 
    // Stage 1: Save message to string with buffer AT Command
    if (newMessageReady && !isMessagePrepared && !isMessageBuffered && !isSendPending) {
        String satComPreppedMessage = "AT+SBDWT=" + satMessageContent;
        newMessageReady = false;
        isMessagePrepared = true;
        HM10Serial.println("Sat Com message string prepped.");
    }
    // Stage 2 - Attempt to buffer the message if not yet buffered
    if (!newMessageReady && isMessagePrepared && !isMessageBuffered && !isSendPending) {
        satComSerial.println(satComPreppedMessage);
        if (satComSerial.available()) {
            String response = satComSerial.readStringUntil('\n');
            if (response.indexOf("OK") != -1) {
                isMessageBuffered = true;
                HM10Serial.println("Sat Com message buffered.");
            } else {
                isMessageBuffered = false;  // Retry buffering on next loop
                HM10Serial.println("Sat Com message buffer failed.");
            }
        }
    }
    // Stage 3: Initiate sending
    if (isMessageBuffered && !isSendPending) {
        satComSerial.println(commandSendSBDIX);
        isSendPending = true;
        attemptCount++; // On the first run, this goes from 0 to 1.
        if (attemptCount == 1) { //If this is the first attempt
            HM10Serial.println("Sat Com message send initiated.");
        } else { // If the send failed (determined by next phase when checking first number of reply)
            HM10Serial.print("Satcom send failed. Retrying. Attempt number: ");
            HM10Serial.println(attemptCount);
        }
    }
    // Stage 4: Check send response if pending
    if (isSendPending) {
        if (satComSerial.available()) {
            String response = satComSerial.readStringUntil('\n');
            // Parse the first number of +SBDIX response
            if (response.startsWith("+SBDIX:")) {
                int firstComma = response.indexOf(',');
                int statusCode = response.substring(8, firstComma).toInt();
                if (statusCode == 0) {
                    // Successful transmission
                    isSendPending = false;  // Message sent, so send is no longer pending
                    isMessageBuffered = false;  // Buffer automatically clears after successful send
                    attemptCount = 0;  // Reset the attempt counter
                    HM10Serial.println("Sat Com message sent successfully.");
                } else {
                    // If status code isn’t zero, retry sending in next loop
                    isSendPending = false;
                    isMessageBuffered = true;  // Still buffered since send failed; retry on next loop
                }
            }
        }
    }


    // If too many send attempts, clear the buffer and reset flags
    if (attemptCount > 50) {
        satComSerial.println(commandClearBuffer);  // Clear buffer, give up until new message requested
        newMessageReady = false;
        isMessageBuffered = false;
        isSendPending = false;
        isMessagePrepared = false;
    }
}

//Needs lots of work, debug via HM20

void sat_com_RX() {
    // Check if data is available to read from the HM-10
    if (satComSerial.available()) {
        String receivedData = satComSerial.readStringUntil('\n'); // Read the incoming data until newline
        receivedData.trim(); // Remove any whitespace or newline characters
        Serial.print("Received: "); // Print the complete message
        Serial.println(receivedData);

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
        } 
    }
}