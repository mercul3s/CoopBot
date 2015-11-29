#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

// Sensor pins 
const int lightSensorPin = 0;
const int tempSensorPin = 1;

// LED pins for temperature visualization
const int redLedPin = 3;
const int grnLedPin = 5;
const int bluLedPin = 6;
const unsigned long timeDelay = 10000;

// Variables for sensor readings
int lightLevel, high = 1023, low = 0;
int lastLightLevel;
float tempLevel, lastTempF, degreesF;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x60, 0x8E };
char server[] = "www.google.com";
IPAddress ip(192, 168, 75, 246);
EthernetClient client;

// Timer for loop timing/delay
unsigned long timer;

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

void setupEthernet() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  // use micros timer for this
  delay(1000);
  Serial.println("Connecting to Ethernet...");

  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
}

void readRequestResponse() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);  
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting");
    client.stop();
  }
}

void sendValues() {
  // send temp, light, and door open/close values to event collector
}

float getTempSensorVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}

float formatTempF(float tempVoltage) {
  float degreesC = (tempVoltage - 0.5) * 100.0;
  float degreesF = degreesC * (9.0/5.0) + 32.0;
  return degreesF;
}

float checkLastTempVal(float currentTempF) {
   if (abs(currentTempF - lastTempF) >= 1) {
    return currentTempF;
   }
}

void tempColor(float temperature) {

  if (temperature >= 95.0) {
    // Red
    analogWrite(redLedPin, 255);
    analogWrite(grnLedPin, 0);
    analogWrite(bluLedPin, 0);
  }
  else if (temperature <= 94.99 && temperature > 80.00) {
    // Orange/Yellow
    analogWrite(redLedPin, 237);
    analogWrite(grnLedPin, 120);
    analogWrite(bluLedPin, 6);
  }
  else if (temperature <= 79.99 and temperature > 60.00) {
    // Green
    analogWrite(redLedPin, 0);
    analogWrite(grnLedPin, 255);
    analogWrite(bluLedPin, 0);
  }
  else if (temperature <= 59.99 and temperature > 40.00) {
    // Purple
    analogWrite(redLedPin, 255);
    analogWrite(grnLedPin, 0);
    analogWrite(bluLedPin, 255);
  }
  else if (temperature <= 39.99 and temperature > 32.50) {
    // Blue
    analogWrite(redLedPin, 0);
    analogWrite(grnLedPin, 0);
    analogWrite(bluLedPin, 255);
  }
  else if (temperature <= 32.49) {
    // White
    analogWrite(redLedPin, 255);
    analogWrite(grnLedPin, 255);
    analogWrite(bluLedPin, 255);
  }
  else {
    // turn all off
    analogWrite(redLedPin, 0);
    analogWrite(grnLedPin, 0);
    analogWrite(bluLedPin, 0);
  }
}

void readSensors() {
  // Read photo sensor every minute, and format the value for LED output.
  lightLevel = analogRead(lightSensorPin);
  tempLevel = getTempSensorVoltage(tempSensorPin);
  degreesF = formatTempF(tempLevel);
  tempColor(degreesF);
  lastTempF = checkLastTempVal(degreesF);
  
  // loop timing in arduino: at the start of your loop (sensor read), you
  // set a timer to be milliseconds, and it starts counting down for you.
  // you keep checking that timer until it exceeds a value, then read your
  // sensor and start your timer again.
  // timer = millis
  Serial.print("Light Level Voltage:    ");
  Serial.println(lightLevel);
  Serial.print("Temp Sensor Voltage:    ");
  Serial.println(tempLevel);
  Serial.print("Temperature in Degrees: ");
  Serial.println(degreesF);
  lightLevel = tuneLightLevel(lightLevel);
}

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(grnLedPin, OUTPUT);
  pinMode(bluLedPin, OUTPUT);
  Serial.begin(9600);
  setupEthernet();
}

void loop() {
  if (millis() - timer >= timeDelay) {
    readSensors();
    readRequestResponse();
    timer = millis();
  }
}



