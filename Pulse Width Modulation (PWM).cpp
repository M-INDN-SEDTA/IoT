// Pin connected to the base of the transistor/MOSFET
const int fanControlPin = 9;  // Change this to the desired pin

void setup() {
  // Set the fan control pin as an output
  pinMode(fanControlPin, OUTPUT);
}

void loop() {
  // Adjust fan speed by changing the PWM value (0 to 255)
  for (int i = 0; i <= 255; i++) {
    analogWrite(fanControlPin, i);
    delay(10); // Adjust the delay for a smoother transition
  }

  delay(1000); // Wait for a second at full speed

  // Decrease fan speed
  for (int i = 255; i >= 0; i--) {
    analogWrite(fanControlPin, i);
    delay(10); // Adjust the delay for a smoother transition
  }

  delay(1000); // Wait for a second at minimum speed
}
