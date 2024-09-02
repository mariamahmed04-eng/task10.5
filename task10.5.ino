#include <Wire.h>

const int MPU_ADDR = 0x68;  // MPU6050 I2C address

int16_t AccelX, AccelY, AccelZ;
int16_t GyroX, GyroY, GyroZ;
int16_t TempRaw;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Wake up MPU6050, as it starts in sleep mode
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // Access the power management register
  Wire.write(0);     // Set to 0 to wake up the MPU6050
  Wire.endTransmission(true);
}

void loop() {
  // Request sensor data from MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  // Request 14 registers

  // Reading raw data
  AccelX = Wire.read() << 8 | Wire.read();  // Accel X
  AccelY = Wire.read() << 8 | Wire.read();  // Accel Y
  AccelZ = Wire.read() << 8 | Wire.read();  // Accel Z
  TempRaw = Wire.read() << 8 | Wire.read();  // Temperature
  GyroX = Wire.read() << 8 | Wire.read();    // Gyro X
  GyroY = Wire.read() << 8 | Wire.read();    // Gyro Y
  GyroZ = Wire.read() << 8 | Wire.read();    // Gyro Z

  // Convert gyro data to yaw (degree/s)
  float GyroZ_degrees = GyroZ / 131.0;  // Scale factor for GyroZ to convert to degree/s

  // Calculate yaw (basic integration over time)
  static float yaw = 0;
  yaw += GyroZ_degrees * 0.01;  // Assuming loop runs every 10ms (100Hz)

  // Print yaw
  Serial.print("Yaw: ");
  Serial.println(yaw);

  delay(10);  // 10ms delay for 100Hz update rate
}
