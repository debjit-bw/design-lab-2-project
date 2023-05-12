#include <MPU9250_WE.h>
#include <Wire.h>
#include <math.h>
#define MPU9250_ADDR 0x68
#define TARGET_ANGLE 0

MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
unsigned long lastGyroTime;
float timedelta, gyroAngle = 0;
float Kp = 1, Ki = 0, Kd = 0, err, intg = 0, derv, prevErr, PIDout;

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
}

void loop() {
  xyzFloat accRaw = myMPU9250.getAccRawValues();
  xyzFloat accCorrRaw = myMPU9250.getCorrectedAccRawValues();
  xyzFloat gValue = myMPU9250.getGValues();
  float resultantG = myMPU9250.getResultantG(gValue);

  timedelta = (micros() - lastGyroTime) / 1e6;
  lastGyroTime = micros()
  xyzFloat gyrRaw = myMPU9250.getGyrRawValues();
  xyzFloat corrGyrRaw = myMPU9250.getCorrectedGyrRawValues();
  xyzFloat gyr = myMPU9250.getGyrValues();

  accAngle = atan(-gValue.x / sqrt(pow(gValue.y, 2) + pow(gValue.z, 2))) * 180 / PI; // [deg]
  gyroAngleDelta = gyr.z * timedelta;
  if (gyroAngle == 0.0) {
    gyroAngle = accAngle;
  } else {
    gyroAngle += gyroAngleDelta;
  }

  err = TARGET_ANGLE - gyroAngle;
  intg += err * timedelta;
  derv = (err - prevErr) / timedelta;
  prevErr = err;
  PIDout = Kp * err + Ki * intg + Kd * derv;

  Serial.println(PIDout);

//   Serial.println("g values (x,y,z):");
//   Serial.print(gValue.x);
//   Serial.print("   ");
//   Serial.print(gValue.y);
//   Serial.print("   ");
//   Serial.println(gValue.z);

//   Serial.println("Gyroscope Data in degrees/s (x,y,z):");
//   Serial.print(gyr.x);
//   Serial.print("   ");
//   Serial.print(gyr.y);
//   Serial.print("   ");
//   Serial.println(gyr.z);

  Serial.println("*********************************");

  delay(100);
}