// Board: Teensy 4.1
//TAB 1 - MAIN SETUP AND LOOP

#include <Wire.h>  // Manages I2C communication between the Teensy and sensors (e.g., BNO055, GPS).
#include <TinyGPS++.h>  // Library for parsing GPS data. Used for getting latitude, longitude, speed, and other navigational information.
#include <Servo.h>  // Standard Servo library to control servos. Essential for motor controls like steering, rudder, or stabilizers.
#include <IBusBM.h>  // Library for reading IBus protocol from a remote receiver. Ensure this is compatible with the Teensy. 
#include <SD.h>  // Library to interface with SD cards. Used for logging data such as sensor readings, GPS position, or system states.
#include <SPI.h>  // Library for SPI communication. Likely used in conjunction with the SD card module.
#include <ADC.h>  // Library to manage Analog-to-Digital Converter (ADC) inputs. Useful if you're using analog sensors or monitoring power levels (like battery voltage).
#include <Adafruit_Sensor.h>  // Base class for all Adafruit sensors. This is required for using the BNO055 sensor.
#include <Adafruit_BNO055.h>  // BNO055 IMU sensor library for reading orientation, gyroscope, accelerometer, and magnetometer data.
#include <Adafruit_MCP9808.h> //adafruit temperature sensors.


#include "globals.h"  // There are quite a few. When I rebuild, I intend to structure differently.
#include "functions.h"  // Function prototypes.


//Boot-up system sends debug messages to bluetooth and usb serial.
void setup() {
    Serial.begin(115200);
    HM10Serial.begin(115200); // Initialize with RX on pin 8, TX on pin 7
    dualFeedbackln(" ");
    dualFeedbackln(" ");
    dualFeedbackln("BOOTING...");
    
    bluetooth_setup();
    drive_setup();    // Setup ESC control
    servo_setup();    // Setup servo control
    dualFeedbackln("...");

    man_con_setup();   // Setup manual control if needed
    sd_card_setup();  // Initialize SD card
    power_setup(); //Initialize power sensors and control pins
    audio_setup();
    satcom_setup();
    motion_setup();
    dualFeedbackln("...");
    thermal_sensor_setup();
    dualFeedbackln("...");

    gps_setup();       // Initialize GPS
    dualFeedbackln(" ");
    dualFeedbackln("...STARTUP COMPLETE.");
    dualFeedbackln(" ");
}


void loop() {
  
    // Functions to run as often as possible
    bluetoothRX(); // Only runs if new message available, per conditions in function.
    gps_read(); // Only runs if new data available, per conditions in function.
    guidance(); // Calculates heading and executes course adjustments
    controlCheck(); // Check if set to manual control or autopilot
    //motion_read(); // Disabled, current build does not rely on motion data.
    transmit_sbd(); // Runs if a message is waiting to be sent via satellite "short burst data"
    check_thermal(); // Check internal temperatures.

    // Timing variables to determine when their corresponding functions are called
    static unsigned long previousThermalCheckMillis = 0;
    static unsigned long previousDataRecordMillis = 0;
    static unsigned long previousPowerReadingsMillis = 0;

    //Power Readings
    if (currentMillis - previousPowerReadingsMillis >= 500) {previousPowerReadingsMillis = currentMillis; power_readings();}
    //Data Record
    if ((currentMillis - previousDataRecordMillis >= 500)&&(recording == true)) {previousDataRecordMillis = currentMillis; data_record();}
     //Thermal Sensors
    if ((currentMillis - previousThermalCheckMillis >= 2000) && (recording == true)) { previousThermalCheckMillis = currentMillis; check_thermal(); }



    //Autopilot-rudder servos and BLDC ESC controls
    if(!manualOverride && autopilot){ drive_control(); servo_control();}
    //Manual Control via Flysky Receiver
    if(manualOverride && !autopilot){ manual_control();}

    //Data Recording, must be enabled by user.
    if (newFile == true){new_data_file();}




// For testing HM10 output, just sends a pulse message "that"
/*
if (millis() - previousMillis >= 2000) {
    previousMillis = millis();
    HM10Serial.println("that");
    Serial.println("that");
}
*/


// Below is for counting average duration of loops then printing to serial
/*
    currentMillis = millis(); // Get the current millis value
    unsigned long loopDuration = currentMillis - previousMillis; // Calculate the duration of the loop
    loopDurationSum += loopDuration; // Accumulate loop durations
    loopCount++; // Increment the loop count
    // Update previousMillis for the next iteration
    previousMillis = currentMillis;
    // Calculate average loop duration every 10 loops
    if (loopCount % 10 == 0) {
        unsigned long averageLoopDuration = loopDurationSum / loopCount;
        Serial.print("Average loop duration: ");
        Serial.print(averageLoopDuration);
        Serial.println(" ms");
    }
*/
}