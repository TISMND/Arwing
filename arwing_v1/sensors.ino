

//----------------------------------------------------
//------------------THERMAL SENSORS-------------------
//----------------------------------------------------


void thermal_sensor_setup() {
  Wire.begin();

// Initialize sensor 1 with its defined address
if (!tempSensor1.begin(SENSOR1_ADDRESS)) {
    dualFeedback("MCP9808 temp sensor 1 not detected.");
} else {
    dualFeedbackln("Temp Sensor 1 online.");
    tempSensor1.setResolution(3); // Set to highest resolution (0.0625°C)
}

// Initialize sensor 2 with its defined address
if (!tempSensor2.begin(SENSOR2_ADDRESS)) {
    dualFeedback("MCP9808 temp sensor 2 not detected.");
} else {
    dualFeedbackln("Temp Sensor 2 online.");
    tempSensor2.setResolution(3); // Set to highest resolution (0.0625°C)
}

}

//Disabled for current testing phase.
void check_thermal() {
 /*
  // Read temperature from sensor 1
  float temperature1 = tempSensor1.readTempC();
  HM10Serial.print("Sensor 1 Temp: ");
  HM10Serial.print(temperature1);
  HM10Serial.println(" °C");
  
 // Serial.print("Sensor 1 Temp: ");
  //Serial.println(temperature1);

  // Read temperature from sensor 2
  float temperature2 = tempSensor2.readTempC();
  HM10Serial.print("Sensor 2 Temp: ");
  HM10Serial.print(temperature2);
  HM10Serial.println(" °C");

//Serial.print("Sensor 2 Temp: ");
//Serial.println(temperature2);
*/
}




//---------------------------------------------
//----------------POWER SENSORS----------------
//---------------------------------------------

void power_setup() {
  pinMode(currDrawSensorPin, INPUT);


  // Configure ADC settings
  adc->adc0->setAveraging(4);  // Some averaging for stability
  adc->adc0->setResolution(12);  // set resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);  // High conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);  // High sampling speed
  adc->adc0->setReference(ADC_REFERENCE::REF_3V3);  // Internal 3.3V reference
}

void power_readings() {
  int rawValue = adc->adc0->analogRead(currDrawSensorPin);
  //Serial.println(rawValue);
  float voltage = (rawValue / 4096.0) * 3.3; // Convert raw value to voltage for 12-bit resolution
  //Serial.println(voltage);
  currentDraw = (voltage - 1.65)/1.65 * 30; //
  //Serial.println(currentDraw);
  const float a = 0.0050;
  const float b = 1.1943;
  const float c = -1.8652;
  // Correct the current draw using polynomial coefficients
  currentDraw = a * currentDraw * currentDraw + b * currentDraw + c;
  currentDraw = currentDraw + 1.26; //Final correction based on gathered data
  //Serial.print("Corrected Current Draw: ");
  //Serial.println(currentDraw, 2);  
  //Serial.println(" A");
}




//---------------------------------------------------
//---------------------MOTION SENSOR-----------------
//---------------------------------------------------

void motion_setup() {
  
  if (!bno.begin()) {
    dualFeedbackln("Iniertial Measurement Unit not detected.");
  }
  else{
    dualFeedbackln("Iniertial Measurement Unit online.");
  }

  bno.setExtCrystalUse(true); // Use external crystal for better accuracy
}


void motion_read() {
  sensors_event_t orientationData, angVelocityData, accelData, magData, linearAccelData, gravityData;


  // Get orientation (euler angles) data
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  //--------------------- Data Collection ---------------------
  
// Get orientation (euler angles) data
bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
orientationX = orientationData.orientation.x;
orientationY = orientationData.orientation.y;
orientationZ = orientationData.orientation.z;
// Get angular velocity (gyroscope) data
bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
angVelocityX = angVelocityData.gyro.x;
angVelocityY = angVelocityData.gyro.y;
angVelocityZ = angVelocityData.gyro.z;
// Get acceleration data
bno.getEvent(&accelData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
accelX = accelData.acceleration.x;
accelY = accelData.acceleration.y;
accelZ = accelData.acceleration.z;
// Get magnetic field data
bno.getEvent(&magData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
magX = magData.magnetic.x;
magY = magData.magnetic.y;
magZ = magData.magnetic.z;
// Get linear acceleration data
bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
linearAccelX = linearAccelData.acceleration.x;
linearAccelY = linearAccelData.acceleration.y;
linearAccelZ = linearAccelData.acceleration.z;
// Get gravity data
bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
gravityX = gravityData.acceleration.x;
gravityY = gravityData.acceleration.y;
gravityZ = gravityData.acceleration.z;

//--------------------- Data Output ---------------------
/*
// Print orientation data
HM10Serial.print("Orientation - X: "); HM10Serial.print(orientationX);
HM10Serial.print(" Y: "); HM10Serial.print(orientationY);
HM10Serial.print(" Z: "); HM10Serial.println(orientationZ);
// Print angular velocity data
HM10Serial.print("Angular Velocity - X: "); HM10Serial.print(angVelocityX);
HM10Serial.print(" Y: "); HM10Serial.print(angVelocityY);
HM10Serial.print(" Z: "); HM10Serial.println(angVelocityZ);
// Print acceleration data
HM10Serial.print("Acceleration - X: "); HM10Serial.print(accelX);
HM10Serial.print(" Y: "); HM10Serial.print(accelY);
HM10Serial.print(" Z: "); HM10Serial.println(accelZ);
// Print magnetic field data
HM10Serial.print("Magnetic Field - X: "); HM10Serial.print(magX);
HM10Serial.print(" Y: "); HM10Serial.print(magY);
HM10Serial.print(" Z: "); HM10Serial.println(magZ);
// Print linear acceleration data
HM10Serial.print("Linear Acceleration - X: "); HM10Serial.print(linearAccelX);
HM10Serial.print(" Y: "); HM10Serial.print(linearAccelY);
HM10Serial.print(" Z: "); HM10Serial.println(linearAccelZ);
// Print gravity data
HM10Serial.print("Gravity - X: "); HM10Serial.print(gravityX);
HM10Serial.print(" Y: "); HM10Serial.print(gravityY);
HM10Serial.print(" Z: "); HM10Serial.println(gravityZ);
*/
}
