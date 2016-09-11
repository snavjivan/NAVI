const int shortFreq = 131;
const int longFreq = 262;
const int lowBattFreq = 65;

const int shortMax = 75;
const int longMax = 175;
const int minDist = 3;

boolean shortMode = true;

const int shortDelay = 25;
const int longDelay = 10;

const int distModePin = 6;
const int feedbackModePin = 7;
int distModePinState;
int feedbackModePinState;

const int audioPin = 5;
const int pingPin = 4;
const int vibePin = 8;
const int lowBattPin = 9;

int maxDist = shortMax;
int beepFreq = shortFreq;
int delayFactor = shortDelay;
int feedbackButtonCount = 0;

boolean hapticMode = true;
boolean audioMode = true;

unsigned int duration;
unsigned int distance;

int numReadings = 5;
int readings[5];
int readIndex = 0;
int total = 0;

unsigned long currentMillis;
int feedbackDelay;

void setup() {
  //declare pins for input or output
  pinMode(lowBattPin, INPUT);
  pinMode(distModePin, INPUT);
  pinMode(feedbackModePin, INPUT);
  pinMode(audioPin, OUTPUT);
  //check if the battery is low or normal
  checkBattery();
  //prepare the array used to smooth the distance values
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  //begin serial communication
  Serial.begin(9600);
}

void loop() {
  //check input from the mode buttons
  checkModes();
  //send a pulse from the ping))) and record its duration
  ping();
  //calculate the distance of the object from the ping)))
  calculateDist();
  //give the appropriate feedback based on modes
  if (distance > maxDist) {
    //distances greater than the max distance are disregarded
  } else if (distance <= maxDist) {
    if (hapticMode == true) {
      hapticFeedback();
    }
    if (audioMode == true) {
      audioFeedback();
    }
    delay(50);
    //stop the feedback
    hapticStopFeedback();
    audioStopFeedback();
    //calculate the delay factor
    feedbackDelay = distance * (1.0 * delayFactor);
    //delay the next iteration of the loop based on distance
    currentMillis = millis();
    wait();
  }
}

void checkModes() {
  checkFeedbackMode();
  checkDistanceMode();
}

void checkFeedbackMode() {
  //input the voltage in the feedback button circuit
  feedbackModePinState = digitalRead(feedbackModePin);
  //switch to the next mode if the button has been pressed
  if (digitalRead(feedbackModePin) != LOW) {
    Serial.println("success");
    if (feedbackButtonCount == 0) {
      audioMode = true;
      hapticMode = false;
    } else if (feedbackButtonCount == 1) {
      audioMode = false;
      hapticMode = true;
    } else if (feedbackButtonCount == 2) {
      audioMode = true;
      hapticMode = true;
    }
    delay(10);
    feedbackButtonCount++;
    //reset the counter to 0 if it is greater than 2
    if (feedbackButtonCount > 2) {
      feedbackButtonCount = 0;
    }
  }
}

void checkDistanceMode() {
  //input the voltage in the distance mode button circuit
  distModePinState = digitalRead(distModePin);
  //switch to the next mode if the button has been pressed
  //assign values of next mode to mode-dependent variables
  if (distModePinState == HIGH) {
    if (shortMode == true) {
      maxDist = longMax;
      beepFreq = longFreq;
      delayFactor = longDelay;
      shortMode = false;
    } else if (shortMode == false) {
      maxDist = shortMax;
      beepFreq = shortFreq;
      delayFactor = shortDelay;
      shortMode = true;
    }
    delay(10);
    //play 3 notes in the tone of the mode
    //to confirm the change to the user
    tone(audioPin, beepFreq, 100);
    delay(100);
    tone(audioPin, beepFreq, 100);
    delay(100);
    tone(audioPin, beepFreq, 100);
    delay(750);
  }
}

void checkBattery() {
  //if the battery is low, play 3 low notes
  if (digitalRead(lowBattPin) == LOW) {
    tone(audioPin, lowBattFreq, 100);
    delay(100);
    tone(audioPin, lowBattFreq, 100);
    delay(100);
    tone(audioPin, lowBattFreq, 100);
    delay(750);
  } //otherwise play a trill of 3 high notes
  else {
    tone(audioPin, shortFreq, 100);
    delay(100);
    tone(audioPin, longFreq, 100);
    delay(100);
    tone(audioPin, 523, 100);
    delay(750);
  }
}

void ping() {
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  //gives 5 microsecond ultrasonic pulse
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  //records duration for pulse to return
  duration = pulseIn(pingPin, HIGH);
}

void calculateDist() {
  distance = duration / 29 / 2;
  if (distance <= maxDist) {
    smoothDistance();
  }
}

void smoothDistance() {
  total = total - readings[readIndex];
  readings[readIndex] = distance;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  distance = total / numReadings;
}

void audioFeedback() {
  //play a note in the mode frequency
  tone(audioPin, beepFreq);
}

void audioStopFeedback() {
  //stop playing the note
  noTone(audioPin);
}

void hapticFeedback() {
  //calculate a duty cycle for PWM based on distance
  int duty = map(distance, minDist, maxDist, 255, 170);
  //use PWM on the motor with the duty cycle
  analogWrite(vibePin, duty);
}

void hapticStopFeedback() {
  //use PWM with a duty cycle of 0% to stop the motor
  analogWrite(vibePin, 0);
}

void wait() {
  //delay in accordance to feedbackDelay while checking modes
  while ((millis() - currentMillis) < feedbackDelay) {
    delay(10);
  }
}
