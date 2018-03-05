
// motor pin inputs
// eventually figure out which one is "open" or "close" and label with that
const int inA = 13;
const int inB = 12;
const int topSwitch = 6;
int topSwitchState, bottomSwitchState;
bool daylight = false;

// debounce delay
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

void setup() {
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(topSwitch, INPUT);
  digitalWrite(topSwitch, HIGH);
  Serial.begin(9600);
}

// debounce reed switch and set its state if it changes from 0 to 1 or vice
// versa
void debounceReedSwitch(int switchPin, int switchState) {
  // hold the values of the reads for debounce comparison
  int switchPinVal1, switchPinVal2
  // get the current value of the switch
  switchPinVal1 = digitalRead(switchPin);
  // delay 10ms between readings 
  if ((unsigned long)(millis() - lastDebounceTime) > debounceDelay) {
    // get the next switch reading
    switchPinVal2 = digitalRead(switchPin);
    
    // ensure that the two readings are consistent
    if (switchPinVal1 == topSwitchPinVal2) {
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

// change to openDoor and closeDoor
void startMotor(int inputPin, String direction) {
  Serial.println("Starting motor ");
  Serial.Println(direction);
  digitalWrite(inputPin, HIGH);
}

void stopMotor(int inputPin, String direction) {
  Serial.println("Stopping motor ");
  Serial.Println(direction);
  digitalWrite(inputPin, LOW);
}

// if it is daylight
// open the door
// - door should check the reed switch state and only start if it is set to zero
// if it is not daylight
// close the door
// - door should check the reed switch state and only start if it is set to zero
void doorCheck() {
  if daylight {
    startMotor(inA, "up");
  }
  startMotor(inB, 
}

void loop() {
  debounceReedSwitch();
}
