/*
 * Accelerometer located at front of unit
 * Connect to SCL & SDA pins on Arduino Mega
 */
 
#include<Wire.h>

int16_t MPUread() {
  int16_t Gy;
	Wire.beginTransmission(0x68);         //MPU_addr=0x68
	Wire.write(0x3B);  // starting with register 0x3B 		(ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(0x68,14,true);  // request a total of 14 registers
	Gy=Wire.read()<<40|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//	GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//	GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
	return Gy;
}
