




//Thermal - MCP9808
const uint8_t SENSOR1_ADDRESS = 0x18; //Default address
const uint8_t SENSOR2_ADDRESS = 0x19; //Solder-modified temperature address
Adafruit_MCP9808 tempSensor1;
Adafruit_MCP9808 tempSensor2;
float temperature1;
float temperature2;

// SATELLITE COMMUNICATION -- ROCKBLOCK
String satMessageContent = "msg placeholder";  // Custom message string
String satComPreppedMessage = "msg placeholder";
bool newMessageReady = false;
bool isMessageBuffered = false;
bool isSendPending = false;
bool isMessagePrepared = false;
String commandSendSBDIX = "AT+SBDIX";
String commandClearBuffer = "AT+SBDD0";
int attemptCount = 0;



//MOTION
Adafruit_BNO055 bno = Adafruit_BNO055(55); // Create BNO055 object
float orientationX, orientationY, orientationZ; // Global variables for orientation data
float angVelocityX, angVelocityY, angVelocityZ; // Global variables for angular velocity data
float accelX, accelY, accelZ; // Global variables for acceleration data
float magX, magY, magZ; // Global variables for magnetic field data
float linearAccelX, linearAccelY, linearAccelZ; // Global variables for linear acceleration data
float gravityX, gravityY, gravityZ; // Global variables for gravity data


// SERIAL PORTS IN USE
using satCom = decltype(Serial4); //SAT COM (rockBLOCK) -- RX 16, TX 17
satCom& satComSerial = Serial4; 
using serialGPS = decltype(Serial7); //GPS (GT-U7) -- RX 28, TX 29
serialGPS& GPSSerial = Serial7; 
using serialHM10 = decltype(Serial8); //BLUETOOTH (HM10) RX 34, TX 35
serialHM10& HM10Serial = Serial8; 
using serialibus = decltype(Serial5); //ibus (flyky receiver) RX 21, TX 20
serialibus& ibusSerial = Serial5; 


// POWER SENSOR AND CONTROL VARS----------
//SENSOR PINS
const int currDrawSensorPin = 41; // Actual: A17. Analog signal from sensor with -30A to +30A range.  SSWWAAAPPPPPPED DURING TROUBLESHOOTING VERSUS DIAGRAM
ADC *adc = new ADC();  // Create an ADC object (For special code that messes with resolution)
//Power Sensor Readings
float batteryVoltage = 0.0; // Placeholder for battery A voltage 
float batteryPercent = 0.0; // Placeholder for battery A voltage percentage 
float currentDraw = 0.0; //The current being drawn from the battery system to power the boat
const float minVoltage = 6.4; //Minimum voltage for 2S lipo
const float maxVoltage = 8.4; //Maximum voltage for 2S lipo



//SD CARD------
File dataFile;
const int chipSelect = BUILTIN_SDCARD;

//idk what these are for.
extern int steeringAngle;
extern int pulseWidth;

//Speaker Pin---
const int speakerPin = A12;  // Speaker connected to pin A12 (pin 26)


//SERVOS (rudders)-----
Servo rudder1;  // Create a Servo object
Servo rudder2;  // Create a Servo object
const int rudder1Pin = 10;  // If issue, confirm rig wire integrity
const int rudder2Pin = 10;  // If issue, confirm rig wire integrity
int midAngle = 90; // Servo center degrees (mid between 0 and 180)
int turnAngle = 40; // Number of degrees the servo will turn during an autopilot correction
int rudderPosition = 90; //Starts at center.
bool adaptive_active = false; //


// ESC DRIVE CONTROL 
Servo esc1;  //says servo because that's the library used, but called "esc" because that's what we're controlling.
Servo esc2;
const int esc1Pin = 9;
const int esc2Pin = 2;   // If issue, confirm rig wire integrity

// Standard RC pulse widths in microseconds
const int minPulseWidth = 1000;    // Minimum throttle
const int maxPulseWidth = 2000;    // Maximum throttle
const int neutralPulseWidth = 1500; // Neutral throttle
float throttle;

// Autopilot esc drive variables
float baseSpeedCrawl = 0.15, baseSpeedCruise = 0.3, baseSpeedBurn = 0.6;
String currentMode = "neutral";
// Autopilot Order (IF it drives, it will do this. As opposed to patrol, recon, etc.)
String currentOrder = "move";


// Manual and autopilot control flags
IBusBM iBusReceiver;  
bool manualOverride = true;
bool autopilot = false;
int reverse = 0;
int throttleRX = 0;
int steeringValue = 0;
int switchDValue = 0;

// GPS and navigation variables
TinyGPSPlus gps;
double latitude = 0.0;
double longitude = 0.0;
double speedKnots = 0.0;  // Speed in knots
double startingLatitude = 0.0;
double startingLongitude = 0.0;
// Target coordinates/bearing variables
double targetLatitude = 0.0;
double targetLongitude = 0.0;
double courseOverGround = 0.0;
double bearingDifference = 0.0;
double targetBearing = 0.0; 
int destAreaMeters = 2; // Destination Diameter in meters, editable by user via SERIAL (HM10 or other) Guidance_calculation() will convert to LL degrees
float destAreaDeg = destAreaMeters*1e-5; // declaring it here so it's global, but it will be updated in guidance right before checking if it's arrived at destination.


// Guidance calculation variables
float initialBearingDifference = 0.0;
bool isCorrectionInProgress = false;
float totalTurnedDegrees = 0.0;
enum CorrectionDirection { Left, Right };
CorrectionDirection correctionDir;
float degCorrectionNeeded = 5.0;
//float compensationPercent = 0.8; - Obsolete, from when turn duration was dictated by accelerometer degrees turned.

// Timing and control
unsigned long timeNow = millis();  //possible artifact
unsigned long lastUpdateTime = 0.0; //used in servo control
unsigned long currentMillis = millis();

// Maneuvering Modifiers (can be changed via bluetooth for calibration in the field)
unsigned long corrDuration = 200; // milliseconds spent turning to correct course (used in guidance calculation)
// unsigned long corrModifier = 0.2; // Percentage of the servo or turning device's maximum angle. (Used in guidance) OBSOLETE
unsigned long straightDuration = 800; //milliseconds spent driving straight before checking if correction is needed (Used in guidance)
unsigned long corrTimer = 0; //Used to track millis() since a turn started
unsigned long straightTimer = 0; //used to track millis() of straight travel
unsigned long corrStart = 0;
unsigned long straightStart = 0;

//BLUETOOTH/HM-10/COMMS Output Bools
//Global vars used to determine if corresponding reading should be transmitted via HM10 serial
bool outputCurrent = false; 
bool outputTemp = false; 
bool outputBat = false; 
bool outputSpeed = false; 
//Output interval timer
float previousOutputMillis = millis();

//For checking loop duration (corresponding code usually commented-out in bottom of main loop)
unsigned long previousMillis = 0;  // Variable to store the previous millis value
unsigned long loopDurationSum = 0; // Variable to accumulate loop durations
unsigned long loopCount = 0;        // Variable to count the number of loops

//Data Recording on SD Card Vars
bool recording = false;
bool newFile = true;
String fileName = "defaultDatalog.txt";





