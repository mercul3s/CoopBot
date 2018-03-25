#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>

// motor pin inputs
// eventually figure out which one is "open" or "close" and label with that
const int inA = 13;
const int inB = 12;
const int topSwitch = 6;

// WIFI setup
const char mySSID[] = "Puppet Guest";
const char myPSK[] = "argon4949";

// State variables
int topSwitchState, bottomSwitchState;
bool daylight = false;

// debounce delay
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

// ********************************************
// Rough outline of steps - x indicates done
//  [x] Connect to WIFI https://learn.sparkfun.com/tutorials/esp8266-wifi-shield-hookup-guide
//  [ ] Sync with NTP server https://www.pjrc.com/teensy/td_libs_Time.html#ntp https://playground.arduino.cc/Code/NTPclient
//  [ ] Startup webserver 
//    [ ] accessible at port 80
//    [ ] returns json payload with door up/down and time info https://arduinojson.org/example/
//  [x] Motor stop/start code 
//        http://www.icstation.com/l298n-motor-driver-board-module-robot-dual-bridge-arduino-p-2707.html
//        https://hackerstore.nl/PDFs/Tutorial298.pdf
//  [ ] use Dusk2Dawn lib to check for sunrise and sunset times https://github.com/dmkishi/Dusk2Dawn
//    [ ] check against current time https://playground.arduino.cc/Code/Time
//    [ ] open door if it is 30 minutes past dawn
//    [ ] close door if it is 30 minutes past dusk (maybe adjust this - see when chickens usually go inside)
//  [ ] chicken door reference http://davenaves.com/blog/interests-projects/chickens/chicken-coop/arduino-chicken-door/
// ********************************************

void setup() {
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(topSwitch, INPUT);
  digitalWrite(topSwitch, HIGH);
  Serial.begin(9600);
  initializeWifi();
  connectWifi();
}


// debounce reed switch and set its state if it changes from 0 to 1 or vice
// versa
void debounceReedSwitch(int switchPin, int switchState) {
  // hold the values of the reads for debounce comparison
  int switchPinVal1, switchPinVal2;
  // get the current value of the switch
  switchPinVal1 = digitalRead(switchPin);
  // delay 10ms between readings 
  if ((unsigned long)(millis() - lastDebounceTime) > debounceDelay) {
    // get the next switch reading
    switchPinVal2 = digitalRead(switchPin);
    
    // ensure that the two readings are consistent
    if (switchPinVal1 == switchPinVal2) {
      if (switchPinVal1 != switchState) {
        // update the switch state, as it has changed
        switchState = switchPinVal1;
      }
      
      Serial.println(" Top Switch Value: ");                
      Serial.println(digitalRead(switchPinVal1));         // display current value of bottom switch;
      // switch alternates between 0 and 1 - turn off/on stop motor based on this value (when it gets to 1, stop motor)
    }
  }
}


void initializeWifi()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin();
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(test);
  }
  Serial.println(F("ESP8266 Shield Present"));
}

// Connect to wifi ssid. Ensure that the shield is in "station" mode so it only connects to wifi,
// not acting as an access point.
void connectWifi()
{
  // esp8266.getMode() checks the current mode; if it's not in station mode, set it to station mode.
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  {
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();

  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
    }
  }

  IPAddress myIP = esp8266.localIP();

  Serial.print(F("My IP: ")); Serial.println(myIP);
}

// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  Serial.println(F("Looping forever."));
  for (;;)
    ;
}

// change to openDoor and closeDoor
void startMotor(int inputPin, String direction) {
  Serial.println("Starting motor ");
  Serial.println(direction);
  digitalWrite(inputPin, HIGH);
}

void stopMotor(int inputPin, String direction) {
  Serial.println("Stopping motor ");
  Serial.println(direction);
  digitalWrite(inputPin, LOW);
}

// if it is daylight
// open the door
// - door should check the reed switch state and only start if it is set to zero
// if it is not daylight
// close the door
// - door should check the reed switch state and only start if it is set to zero
void doorCheck() {
  if (daylight) {
    startMotor(inA, "up");
  }
  startMotor(inB, "down");
}

void loop() {
  //debounceReedSwitch();
}
