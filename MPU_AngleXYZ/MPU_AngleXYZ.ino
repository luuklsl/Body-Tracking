#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial xbee(8, 9);


const int MPU_addr = 0x68;  int16_t AcX,  AcY,  AcZ,  Tmp,  GyX,  GyY,  GyZ;
const int MPU2_addr = 0x69; int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

float xavg, yavg, zavg,xavg2, yavg2, zavg2;
int minVal = 265; int maxVal = 402;
int i,j, xval, yval, zval; 
int avgx[10],avgy[10],avgz[10],avgx2[10],avgy2[10],avgz2[10];  //initializing arrays for writig down 10 latest measurments

double x;  double y;  double z;
double x2; double y2; double z2;

void setup() {
  xbee.begin(9600);
  Wire.begin();
  i=0;

  //MPU1
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

  //MPU2
    Wire.beginTransmission(MPU2_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
  
  Serial.begin(9600);
}
boolean calibrated = false;
double xc,yc,zc,xc2,yc2,zc2;
void loop() {
  //MPU_1
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); //accelerometer 
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
    Wire.write(0x3B); //accelerometer 
    Wire.endTransmission();
    Wire.requestFrom(MPU2_addr, 14);
    AcX2 = Wire.read() << 8 | Wire.read();
    AcY2 = Wire.read() << 8 | Wire.read();
    AcZ2 = Wire.read() << 8 | Wire.read();
  
    int xAng2 = map(AcX2, minVal, maxVal, -90, 90);
    int yAng2 = map(AcY2, minVal, maxVal, -90, 90);
    int zAng2 = map(AcZ2, minVal, maxVal, -90, 90);
  
    x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2) + PI);
    y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2) + PI);
    z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2) + PI);

//  //Printing
//    xval=x2-x; yval=y2-y; zval=z2-z; //calcualting difference in values between 2 MPUs for X,Y and Z
//    if(xval<0){
//      xval+=360;}
//    if(yval<0){
//      yval+=360;}
//    if(zval<0){
//      zval+=360;
//      }
//      


    //Writing current difference value into an array  
      avgx[i]=x; avgy[i]=y; avgz[i]=z;
      avgx2[i]=x2; avgy2[i]=y2; avgz2[i]=z2;
 
    //Store and display the last 10 difference values 
      if(i==9){
        //Serial.println("VALUES IN THE ARRAY");
        long sumx, sumy, sumz = 0L ;  // sum will be larger than an item, long for safety.
        long sumx2, sumy2, sumz2 = 0L; 
          for(j=0; j<10; j++){
            sumx += avgx [j]; sumy += avgy [j]; sumz += avgz [j]; //Find sum of last 10 diference values
            sumx2 += avgx2 [j]; sumy2 += avgy2 [j]; sumz2 += avgz2 [j]; //Find sum of last 10 diference values
            
          }
        
        //Find avarage value from the last 10 differences
          xavg= sumx/(i+1); yavg= sumy/(i+1); zavg= sumz/(i+1);
          xavg2= sumx2/(i+1); yavg2= sumy2/(i+1); zavg2= sumz2/(i+1);
        
//        //Print the average
//        if (xavg>350 || xavg<10){
//          xavg = 0;
//        }
//        if (yavg>350 || yavg<10){
//          yavg = 0;
//        }
//        if (zavg>350 || zavg<10){
//          zavg = 0;
//        }
//         if (xavg2>350 || xavg2<10){
//          xavg2 = 0;
//        }
//        if (yavg2>350 || yavg2<10){
//          yavg2 = 0;
//        }
//        if (zavg2>350 || zavg2<10){
//          zavg2 = 0;
//        }

//        Serial.print("Angle");Serial.print("\t");Serial.println(xavg); 
//        Serial.println("-----------------------------------------");
//        xbee.print(String(xavg) + "," + String(yavg) + "," + String(zavg) + "b");
//        delay(166);

        if(!calibrated){
          calibrated = true;
          xc = xavg;
          yc = yavg;
          zc = zavg;
          xc2 = xavg2f ri 0p ';
          yc2 = yavg2;
          zc2 = zavg2;
        }
        xavg -= xc;
         yavg -= yc;
          zavg -= zc;
        xavg2 -= xc2;
         yavg2 -= yc2;
          zavg2 -= zc2;       
        xbee.print(String(xavg) + "," + String(yavg) + "," + String(zavg) + "b");
        xbee.print(String(xavg2) + "," + String(yavg2)+ "," + String(zavg2) + "c");
        delay(166);

       i=-1; //reset counter 
    }


i++;
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

