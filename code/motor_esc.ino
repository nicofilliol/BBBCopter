#include <Servo.h>

Servo ESC; // Create a servo object to control the ESC

void setup() {
  // Put your setup code here, to run once:
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
}

void loop() {
  // Put your main code here, to run repeatedly:
   int potValue = analogRead(A0);   // Reads the value of the potentiometer (value between 0 and 1023)
   potValue = map(potValue, 0, 1023, 0, 180);   // Scale it for use with the servo library (value between 0 and 180, usually degrees for a servo motor -> corresponds to motor speed in this case)
   ESC.write(potValue); // Send the signal to the ESC
}