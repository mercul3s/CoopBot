// Sensor pins 
const int lightSensorPin = 0;
const int tempSensorPin = 1;

// LED pins for temperature visualization
const int redLedPin = 9;
const int grnLedPin = 10;
const int bluLedPin = 11;

// Variables for sensor readings
int lightLevel, high = 1023, low = 0;
int lastLightLevel;
float tempLevel, lastTempF, degreesF;

// Timer for loop timing/delay
unsigned long timer = 60000;

int tuneLightLevel(int sensorValue) {
  if (sensorValue < low) {
    low = lightLevel;
  }
  if (sensorValue > high) {
    high = lightLevel;
  }
  lightLevel = map(lightLevel, low+30, high-30, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
  return lightLevel;
}

void openCoopDoor() {
  // nothing to see here
}

void closeCoopDoor() {
  // nothing to see here  
}

void sendValues() {
  // nothing to see here
}

float getTempSensorVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}

float formatTempF(float tempVoltage) {
  float degreesC = (tempVoltage - 0.5) * 100.0;
  float degreesF = degreesC * (9.0/5.0) + 32.0;
  return degreesF;
}

void checkLastTempVal(float currentTempF) {
   if (abs(currentTempF - lastTempF) >= 1) {
    lastTempF = currentTempF;
   }
}

void tempColor(float temperature) {

  if (temperature >= 95.0) {
    // Red
    digitalWrite(redLedPin, HIGH);
    digitalWrite(grnLedPin, LOW);
    digitalWrite(bluLedPin, LOW);
   
  }
  else if (temperature <= 94.99 && temperature > 80.00) {
    // Orange/Yellow
    digitalWrite(redLedPin, HIGH);
    digitalWrite(grnLedPin, HIGH);
    digitalWrite(bluLedPin, LOW);
    
  }
  else if (temperature <= 79.99 and temperature > 60.00) {
    // Green
    digitalWrite(redLedPin, LOW);
    digitalWrite(grnLedPin, HIGH);
    digitalWrite(bluLedPin, LOW);
    
  }
  else if (temperature <= 59.99 and temperature > 40.00) {
    // Purple (turn red and blue on):
    digitalWrite(redLedPin, HIGH);
    digitalWrite(grnLedPin, LOW);
    digitalWrite(bluLedPin, HIGH);
  }
  else if (temperature <= 39.99 and temperature > 32.50) {
    // Blue
    digitalWrite(redLedPin, LOW);
    digitalWrite(grnLedPin, LOW);
    digitalWrite(bluLedPin, HIGH);
  }
  else if (temperature <= 32.49) {
    // White
    digitalWrite(redLedPin, HIGH);
    digitalWrite(grnLedPin, HIGH);
    digitalWrite(bluLedPin, HIGH);
  }
  else {
    // turn all off
    digitalWrite(redLedPin, LOW);
    digitalWrite(grnLedPin, LOW);
    digitalWrite(bluLedPin, LOW);
  }
}

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(grnLedPin, OUTPUT);
  pinMode(bluLedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read photo sensor every minute, and format the value for LED output.
  // If there is a change in temp greater than 1 degree, set it to lastTempF
  // Log this value later.
  // Need to get a good baseline of light/dark values for opening coop door
  lightLevel = analogRead(lightSensorPin);
  tempLevel = getTempSensorVoltage(tempSensorPin);
  degreesF = formatTempF(tempLevel);
  tempColor(degreesF);
  checkLastTempVal(degreesF);
  
  // loop timing in arduino: at the start of your loop (sensor read), you
  // set a timer to be milliseconds, and it starts counting down for you.
  // you keep checking that timer until it exceeds a value, then read your
  // sensor and start your timer again.
  // timer = millis
  Serial.println("Light Level Voltage");
  Serial.println(lightLevel);
  Serial.println("Temp Sensor Voltage");
  Serial.println(tempLevel);
  Serial.println("Temperature in Degrees: ");
  Serial.println(degreesF);
  
  lightLevel = tuneLightLevel(lightLevel);
  delay(10000);
}



