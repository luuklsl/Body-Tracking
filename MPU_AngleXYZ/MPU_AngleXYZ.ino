#include <Wire.h>

const int MPU_addr = 0x68;  int16_t AcX,  AcY,  AcZ,  Tmp,  GyX,  GyY,  GyZ;
const int MPU2_addr = 0x69; int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

int minVal = 265; int maxVal = 402;

double x;  double y;  double z;
double x2; double y2; double z2;

void setup() {
  Wire.begin();

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU2_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.begin(9600);
}

void loop() {
  //MPU_1
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU_addr, 14);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  
  //MPU_2
  Wire.beginTransmission(MPU2_addr);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU2_addr, 14);
  AcX2 = Wire.read() << 8 | Wire.read();
  AcY2 = Wire.read() << 8 | Wire.read();
  AcZ2 = Wire.read() << 8 | Wire.read();

  //Printing
  int xAng2 = map(AcX2, minVal, maxVal, -90, 90);
  int yAng2 = map(AcY2, minVal, maxVal, -90, 90);
  int zAng2 = map(AcZ2, minVal, maxVal, -90, 90);

  x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2) + PI);
  y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2) + PI);
  z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2) + PI);

  Serial.print("AngleX= "); Serial.print(x); Serial.print("\t"); Serial.print("X2= "); Serial.println(x2);
  Serial.print("AngleY= "); Serial.print(y); Serial.print("\t"); Serial.print("Y2= "); Serial.println(y2);
  Serial.print("AngleZ= "); Serial.print(z); Serial.print("\t"); Serial.print("Z2= "); Serial.println(z2);
  Serial.println("-----------------------------------------");
  delay(400);
}
/*
void GetMpuValue(const int MPU){
  Wire.beginTransmission(MPU); 
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU,14);
  while(Wire.available() < 14);
  accelX = Wire.read()<<8|Wire.read(); 
  accelY = Wire.read()<<8|Wire.read(); 
  accelZ = Wire.read()<<8|Wire.read();

  int xAng2 = map(AcX2, minVal, maxVal, -90, 90);
  int yAng2 = map(AcY2, minVal, maxVal, -90, 90);
  int zAng2 = map(AcZ2, minVal, maxVal, -90, 90);

  x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2) + PI);
  y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2) + PI);
  z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2) + PI);

  Serial.print("AngleX= "); Serial.print(x); Serial.print("\t"); Serial.print("X2= "); Serial.println(x2);
  Serial.print("AngleY= "); Serial.print(y); Serial.print("\t"); Serial.print("Y2= "); Serial.println(y2);
  Serial.print("AngleZ= "); Serial.print(z); Serial.print("\t"); Serial.print("Z2= "); Serial.println(z2);
  Serial.println("-----------------------------------------");
  delay(400);
  
} */

