#include <MPU9250_WE.h>
#include <Wire.h>
#include <math.h>
#define MPU9250_ADDR 0x68
#include <Servo.h>

float TARGET_ANGLE = 0;
Servo myServo;  // create a servo object
MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
unsigned long lastGyroTime;
float timedelta, gyroAngle = 0, accAngle, gyroAngleDelta;
float err, intg = 0, derv, prevErr, PIDout;
// float Kp = 1, Ki = -0.001, Kd = 0.02;
float Kp = 0.9, Ki = -0.0001, Kd = 0.05;
float prevPID = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  }
  else{
    Serial.println("MPU9250 is connected");
  }

  Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(5000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");

  myMPU9250.setSampleRateDivider(5);
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
  myMPU9250.enableAccDLPF(true);
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);

  myMPU9250.enableGyrDLPF();
  myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
  myMPU9250.setSampleRateDivider(99);
  myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);

  myServo.attach(9);
  myServo.write(90);
  delay(5000);
  // myServo.write(0);
  // delay(5000);
  // myServo.write(180);
  // delay(5000);
}

void loop() {
  xyzFloat accRaw = myMPU9250.getAccRawValues();
  xyzFloat accCorrRaw = myMPU9250.getCorrectedAccRawValues();
  xyzFloat gValue = myMPU9250.getGValues();
  float resultantG = myMPU9250.getResultantG(gValue);

  timedelta = (micros() - lastGyroTime) / 1e6;
  lastGyroTime = micros();
  xyzFloat gyrRaw = myMPU9250.getGyrRawValues();
  xyzFloat corrGyrRaw = myMPU9250.getCorrectedGyrRawValues();
  xyzFloat gyr = myMPU9250.getGyrValues();

  accAngle = atan(-gValue.x / sqrt(pow(gValue.y, 2))) * 180 / PI; // [deg]
  gyroAngleDelta = gyr.z * timedelta;
  if (gyroAngle == 0.0) {
    gyroAngle = accAngle;
    TARGET_ANGLE = gyroAngle;
  } else {
    gyroAngle += gyroAngleDelta;
  }
  Serial.println(gyroAngle);

  err = TARGET_ANGLE - gyroAngle;
  intg += err * timedelta;
  derv = (err - prevErr) / timedelta;
  prevErr = err;
  PIDout = Kp * err + Ki * intg + Kd * derv;

  if (prevPID == 0) {
    prevPID = PIDout;
  }
  // if ((PIDout - prevPID) > 0) {
  //   myServo.write(180);
  // } else {
  //   myServo.write(0);
  // }
  Serial.println(PIDout);
  prevPID = PIDout;
  if (PIDout < 0.1) {
    myServo.write(180);
  } else if (PIDout > 0.1) {
    myServo.write(0);
  }

  // if (gyroAngleDelta < -0.001) {
  //   myServo.write(180);
  // } else if (gyroAngleDelta < +0.001) {
  //   myServo.write(0);
  // }

  // if (gyroAngleDelta < 0) {
  //   myServo.write(180);
  // } else {
  //   myServo.write(0);
  // }


  Serial.println("*********************************");

  delay(5);
}