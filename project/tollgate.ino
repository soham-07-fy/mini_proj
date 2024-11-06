#include <Servo.h>

Servo myServo;
int trigPin = 9;
int echoPin = 10;
long duration;
int distance;

void setup() {
  myServo.attach(3);  // Servo connected to pin 3
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Send out a pulse from the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo time
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (speed of sound = 343 m/s)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Move servo based on object distance
  if (distance < 20 && distance > 0) {  // Object detected within 20 cm
    myServo.write(90);  // Rotate servo to 90°
  } else {
    myServo.write(0);  // No object, servo returns to 0°
  }

  delay(500);  // Wait half a second before the next measurement
}
