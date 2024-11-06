// Define IR sensor pins
const int irSensor1 = A0; // Connect the first IR sensor to analog pin A0
const int irSensor2 = A1; // Connect the second IR sensor to analog pin A1

// Variables for timing
unsigned long startTime;
unsigned long endTime;
float speed; // Speed in m/s
const float distance = 0.05; // Distance between IR sensors in meters (example: 5 cm)
const int threshold = 200; // Threshold for detecting an object (adjust as necessary)

void setup() {
    Serial.begin(9600); // Start the Serial communication at 9600 baud rate
}

void loop() {
    // Read the analog value from each sensor
    int sensor1Value = analogRead(irSensor1);
    int sensor2Value = analogRead(irSensor2);

    // Print the analog values of each sensor
    Serial.print("Sensor 1 Value: ");
    Serial.print(sensor1Value);
    Serial.print(", Sensor 2 Value: ");
    Serial.println(sensor2Value);

    // Check if the first sensor detects an object
    if (sensor1Value < threshold) { // If an object is detected at sensor 1
        delay(100); // Debounce delay to avoid false triggering
        if (analogRead(irSensor1) < threshold) { // Confirm detection
            startTime = micros(); // Record start time in microseconds
            
            // Wait for the object to reach the second IR sensor
            while (analogRead(irSensor2) >= threshold) {
                // Do nothing until the second sensor is triggered
            }
            
            endTime = micros(); // Record end time in microseconds
            float timeTaken = (endTime - startTime) / 1000000.0; // Convert time to seconds
            
            // Calculate speed in m/s
            if (timeTaken > 0) { // Prevent division by zero
                speed = distance / timeTaken; 
                // Convert speed to km/h
                float speedKmH = speed * 3.6;

                // Print speed to Serial Monitor
                Serial.print("Distance: ");
                Serial.print(distance); // Print distance
                Serial.print(" m, Time Taken: ");
                Serial.print(timeTaken, 6); // Print time taken in seconds
                Serial.print(" seconds, Speed: ");
                Serial.print(speedKmH, 2);  // Print speed in km/h to 2 decimal places
                Serial.println(" km/h");
            } else {
                Serial.println("Error: Time taken is zero.");
            }

            delay(2000); // Wait for 2 seconds before the next measurement
        }
    }
    
    delay(500); // Delay for readability
}
