#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_MotorShield motorShield = Adafruit_MotorShield();
Adafruit_DCMotor* motor1 = motorShield.getMotor(1);
Adafruit_DCMotor* motor2 = motorShield.getMotor(2);
Adafruit_BNO055 bno = Adafruit_BNO055();

float Kp = 1.5;
float Ki = 0.5;
float Kd = 0.5;
float last_error = 0;
float integral = 0;
float output;

void setup() {
  Serial.begin(9600);
  motorShield.begin();
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  motor1->run(FORWARD);
  motor2->run(FORWARD);
  bno.begin();
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  float angle = event.orientation.z;
  float error = angle - 180.0;
  integral += error;
  float derivative = error - last_error;

  output = Kp * error + Ki * integral + Kd * derivative;

  last_error = error;

  motor1->setSpeed(abs(output));
  motor2->setSpeed(abs(output));

  if (output > 0) {
    motor1->run(BACKWARD);
    motor2->run(FORWARD);
  } else {
    motor1->run(FORWARD);
    motor2->run(BACKWARD);
  }

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(", Error: ");
  Serial.print(error);
  Serial.print(", Output: ");
  Serial.println(output);

  delay(50);
}