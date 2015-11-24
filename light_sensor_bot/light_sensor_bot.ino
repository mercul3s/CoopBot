
const int lightSensorPin = 0;
const int ledPin = 9;

int lightLevel, high = 1023, low = 0;
int timer = 6000;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read photo sensor every minute, and format the value for LED output.
  // Need to get a good baseline of light/dark values for opening coop door
  lightLevel = analogRead(lightSensorPin);
  Serial.println("Analog Sensor Reading");
  Serial.println(lightLevel);
  lightLevel = tuneLightLevel(lightLevel);
  Serial.println("LED Formatted Sensor Reading");
  Serial.println(lightLevel);
  analogWrite(ledPin, lightLevel);
  delay(timer);
}

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


