#include <SPI.h>
#include <Ethernet.h>

// Sensor pins 
const int lightSensorPin = 0;
const int tempSensorPin = 1;

// LED pins for temperature visualization
const int redLedPin = 3;
const int grnLedPin = 5;
const int bluLedPin = 6;

// Variables for sensor readings
int lightLevel, high = 1023, low = 0;
int lastLightLevel;
float tempLevel, lastTempF, degreesF;

// Ethernet Shield vars
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x60, 0x8E };
char server[] = "requestb.in";
//int serverPort = 80;
//char pageName[] = "/r4sgsfr4";
char sensorData[128];
char tempData[10];
String testData = "chicken=Drumstick";

IPAddress ip(192, 168, 75, 246);
EthernetClient client;

// Timers for loop timing/delay
unsigned long timer;
const unsigned long timeDelay = 10000;

char params[40];
#define delayMillis 30000Ul
unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

// Motor/Door functions
void openCoopDoor() {
  // nothing to see here
}

void closeCoopDoor() {
  // nothing to see here  
}

// HTTP Request and Ethernet Functions
void setupEthernet() {
  Serial.print(F("Starting ethernet..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    Ethernet.begin(mac, ip);
  }
  else {
    Serial.print("IP Address: ");
    Serial.println(Ethernet.localIP());
    delay(1000);
    Serial.println("Ready");
  }
}

byte postData(String data) {
  Serial.print(F("connecting..."));
  
  if(client.connect(server,80)) {
    client.println("POST /r4sgsfr4 HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
  }
  else {
    Serial.println(F("Connection Failed"));
  }

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  Serial.println();
  client.stop();
}

// Temp Sensor functions
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
    //  dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
    dtostrf(lastTempF, 5, 2, tempData);
    Serial.println(tempData);
    printf(sensorData,"temperature=",tempData);
    postData(sensorData);
   }
    Serial.print("Last temp: ");
    Serial.println(lastTempF);\
    Serial.println(sensorData);
}

// Decision tree for temperature LED status light
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
  checkLastTempVal(degreesF);
  
  // loop timing in arduino: at the start of your loop (sensor read), you
  // set a timer to be milliseconds, and it starts counting down for you.
  // you keep checking that timer until it exceeds a value, then read your
  // sensor and start your timer again.
  // timer = millis
  Serial.println();
  Serial.print("Light Level Voltage:    ");
  Serial.println(lightLevel);
  Serial.print("Temp Sensor Voltage:    ");
  Serial.println(tempLevel);
  Serial.print("Temperature in Degrees: ");
  Serial.println(degreesF);
}

// Precursor to magic
void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(grnLedPin, OUTPUT);
  pinMode(bluLedPin, OUTPUT);
  Serial.begin(9600);
  setupEthernet();
}

// This is where the magic happens
void loop() {
  if (millis() - timer >= timeDelay) {
    readSensors();
//    postData(testData);
    timer = millis();
  }
}



