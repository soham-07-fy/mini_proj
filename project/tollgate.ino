#include <Servo.h>

// Pin Definitions for Traffic Lights
#define redLight 13
#define yellowLight 12
#define greenLight 11

// Pin Definitions for Ultrasonic Sensor
#define ultrasonicTrigPin 9
#define ultrasonicEchoPin 8

// Pin Definition for IR Sensor
#define irSensorPin 7

// Pin Definition for Push-Button Switch
#define buttonPin 6 // Pin for the push-button switch

// Servo for Toll Gate
Servo gateServo;
#define servoPin 10  // Pin for Servo motor

long duration;
int distance;
int vehicleDetected;
int buttonState;

void setup() {
  // Initialize traffic light pins
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  
  // Initialize IR sensor pin
  pinMode(irSensorPin, INPUT);
  
  // Initialize the servo motor for toll gate
  gateServo.attach(servoPin);  
  gateServo.write(0);  // Initial position: Gate closed
  
  // Initialize the button pin with internal pull-up
  pinMode(buttonPin, INPUT_PULLUP); 
  
  // Begin serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Measure vehicle presence using IR sensor
  vehicleDetected = digitalRead(irSensorPin);
  
  // Trigger ultrasonic sensor to measure distance
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);
  
  // Read the echo pin and calculate the distance
  duration = pulseIn(ultrasonicEchoPin, HIGH);
  distance = duration * 0.034 / 2;  // Convert to centimeters
  
  // Read the button state
  buttonState = digitalRead(buttonPin);
  
  // Debugging output to serial monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Vehicle detected by IR sensor: ");
  Serial.println(vehicleDetected);
  Serial.print("Button state: ");
  Serial.println(buttonState);
  
  // Check button state for manual control
  if (buttonState == LOW) {  // Button pressed
    gateServo.write(90); // Open gate
    Serial.println("Gate opened manually");
    delay(5000); // Keep the gate open for 5 seconds
    gateServo.write(0); // Close gate
    Serial.println("Gate closed manually");
    delay(1000); // Wait for a second before next action
  } else if (distance < 15 || vehicleDetected == HIGH) {  // If a vehicle is detected within 15 cm or IR sensor is triggered
    // Green light ON (vehicle detected), red light OFF
    digitalWrite(greenLight, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, LOW);
    
    // Open the toll gate (move servo to 90 degrees)
    gateServo.write(90);
    Serial.println("Gate opened automatically");

    // Wait for 5 seconds to simulate vehicle passing
    delay(5000);
    
    // Close the toll gate (move servo back to 0 degrees)
    gateServo.write(0);
    Serial.println("Gate closed automatically");
  } else {
    // Red light ON (no vehicle detected), green light OFF
    digitalWrite(redLight, HIGH);
    digitalWrite(greenLight, LOW);
    digitalWrite(yellowLight, LOW);
  }

  // Small delay for sensor stability
  delay(1000);
}

