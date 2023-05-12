// include the Servo library
#include <Servo.h>

Servo myServo;  // create a servo object

int pos = 0;

void setup() {
  myServo.attach(9);   // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);  // open a serial connection to your computer
}

void loop() {
  Serial.println(0);
  myServo.write(0);
  delay(1000);
  for (pos = 0; pos <= 180; pos += 1) {
    // pos cannot go beyond 180
    Serial.println(pos);
    myServo.write(pos);
    delay(1);
  }
  delay(1000);

  for (pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);
    delay(1);
  }
  delay(1000);
}