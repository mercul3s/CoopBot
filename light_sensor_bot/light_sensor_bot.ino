//#include <BlinkTask.h>
//#include <Debouncer.h>
//#include <DelayRun.h>
//#include <Dimmer.h>
//#include <FrequencyTask.h>
//#include <Heartbeat.h>
//#include <Rotary.h>
//#include <SoftPwmTask.h>
//#include <SoftTimer.h>
//#include <Task.h>
//#include <TonePlayer.h>

// Sensor pins 
const int lightSensorPin = 0;
const int tempSensorPin = 1;
const int ledPin = 9;

// Variables for sensor readings
int lightLevel, high = 1023, low = 0;
int lastLightLevel;
float tempLevel, degreesC, degreesF;

// Timer for loop timing 
unsigned long timer = 60000;

// This code is broken, library doesn't seem to be working
//void readSensor(Task* me) {
//  // Read photo sensor every minute, and format the value for LED output.
//  // Need to get a good baseline of light/dark values for opening coop door
//  lightLevel = analogRead(lightSensorPin);
//  // loop timing in arduino: at the start of your loop (sensor read), you
//  // set a timer to be milliseconds, and it starts counting down for you.
//  // you keep checking that timer until it exceeds a value, then read your
//  // sensor and start your timer again.
//  // timer = millis
//  Serial.println("Analog Sensor Reading");
//  Serial.println(lightLevel);
//  lightLevel = tuneLightLevel(lightLevel);
//  Serial.println("LED Formatted Sensor Reading");
//  Serial.println(lightLevel);
//  analogWrite(ledPin, lightLevel);
//}

//Task t1(timer, readSensor);

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

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
//  SoftTimer.add(&t1);
}

float getTempSensorVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}

float formatTemp(float tempVoltage) {
    
}

void loop() {
  // Read photo sensor every minute, and format the value for LED output.
  // Need to get a good baseline of light/dark values for opening coop door
  lightLevel = analogRead(lightSensorPin);
  tempLevel = getTempSensorVoltage(tempSensorPin);
  degreesC = (tempLevel - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  // loop timing in arduino: at the start of your loop (sensor read), you
  // set a timer to be milliseconds, and it starts counting down for you.
  // you keep checking that timer until it exceeds a value, then read your
  // sensor and start your timer again.
  // timer = millis
  Serial.println("Light Level Voltage");
  Serial.println(lightLevel);
  Serial.println("Temp Sensor Voltage");
  Serial.println(tempLevel);
  Serial.println("Temperature in Celcius: ");
  Serial.println(degreesC);
  Serial.println("Temperature in Degrees: ");
  Serial.println(degreesF);
  
  lightLevel = tuneLightLevel(lightLevel);
  analogWrite(ledPin, lightLevel);
  delay(10000);
}



