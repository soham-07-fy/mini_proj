#include <Servo.h>

// Pin Definitions
const int trigPin = 8;        // Ultrasonic sensor Trig pin
const int echoPin = 9;        // Ultrasonic sensor Echo pin
const int redLED = 10;        // Red LED for stop
const int yellowLED = 11;     // Yellow LED for caution
const int greenLED = 12;      // Green LED for go
const int servoPin = 3;       // Servo motor pin
const int buttonPin = A3;     // Pushbutton pin for manual control

// Variables
long duration;
int distance;
bool gateOpen = false;            // Track gate status
bool lastButtonState = HIGH;      // Track previous button state
bool buttonState;                 // Current button state
unsigned long lastDebounceTime = 0;  // Debounce timer
const unsigned long debounceDelay = 50;  // 50 ms debounce delay

Servo gateServo;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Pin Modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up

  // Attach Servo
  gateServo.attach(servoPin);

  // Start with gate closed and red LED on
  closeGate();
}

void loop() {
  // Handle button input with debouncing
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {  // Button pressed
        if (gateOpen) {
          closeGate();
        } else {
          openGate();
        }
      }
    }
  }
  
  lastButtonState = reading;

  // Measure distance and control LEDs and gate based on distance
  distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= 2) {  // If object is very close, turn on red LED, keep gate closed
    closeGate();
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  } else if (distance > 2 && distance <= 5) {  // Object in caution range, yellow LED on
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  } else if (distance > 5 && distance <= 10) {  // Object in range, green LED on, open gate
    openGate();
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  } else {  // Object out of range, close gate and red LED on
    closeGate();
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }

  delay(50); // Small delay for stability
}

// Function to measure distance using ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

// Function to open the gate
void openGate() {
  if (!gateOpen) {
    gateServo.write(90);  // Rotate servo to open position
    gateOpen = true;
    Serial.println("Gate opened");
  }
}

// Function to close the gate
void closeGate() {
  if (gateOpen) {
    gateServo.write(0);  // Rotate servo to closed position
    gateOpen = false;
    Serial.println("Gate closed");
  }
}
