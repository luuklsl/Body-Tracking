#include <Wire.h>

const int MPU_addr = 0x68;  int16_t AcX,  AcY,  AcZ,  Tmp,  GyX,  GyY,  GyZ;
const int MPU2_addr = 0x69; int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;

float xavg, yavg, zavg;
int minVal = 265; int maxVal = 402;
int i,j, xval, yval, zval; 
int avgx[10],avgy[10],avgz[10]; //initializing arrays for writig down 10 latest measurments

double x;  double y;  double z;
double x2; double y2; double z2;

void setup() {
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

  //Printing
    xval=x2-x; yval=y2-y; zval=z2-z; //calcualting difference in values between 2 MPUs for X,Y and Z
    if(xval<0){
      xval+=360;}
    if(yval<0){
      yval+=360;}
    if(zval<0){
      zval+=360;
      }
      
    //Writing current difference value into an array  
      avgx[i]=xval; avgy[i]=yval; avgz[i]=zval;
        /*Displaying values from the array every step [10 steps]
        Serial.print("[i]="); Serial.println(i);
        Serial.print("Avg X= ");Serial.print(avgx[i]);Serial.print("\t");
        Serial.print("Avg Y= ");Serial.print(avgy[i]);Serial.print("\t");
        Serial.print("Avg Z= ");Serial.print(avgz[i]);Serial.println("\t"); 
        */

    //Store and display the last 10 difference values 
      if(i==9){
        //Serial.println("VALUES IN THE ARRAY");
        long sumx, sumy, sumz = 0L ;  // sum will be larger than an item, long for safety.
          for(j=0; j<10; j++){
            //Serial.print("XYZ");Serial.print(j+1);Serial.print("\t");Serial.print(avgx[j]);Serial.print("\t");Serial.print(avgy[j]);Serial.print("\t");Serial.println(avgz[j]);
            sumx += avgx [j]; sumy += avgy [j]; sumz += avgz [j]; //Find sum of last 10 diference values
          }
        
        //Print the Sums
          //Serial.println();Serial.print("SUM");Serial.print("\t");Serial.print(sumx);Serial.print("\t");Serial.print(sumy);Serial.print("\t");Serial.print(sumz);Serial.println("\t");
        
        //Find avarage value from the last 10 differences
          xavg= sumx/(i+1); yavg= sumy/(i+1); zavg= sumz/(i+1);
        
        //Print the average
        if (xavg>350 || xavg<10){
          xavg = 0;
        }
        if (yavg>350 || yavg<10){
          yavg = 0;
        }
        if (zavg>350 || zavg<10){
          zavg = 0;
        }
          /*Serial.println("-----------------------------------------");
          Serial.print("AVG");Serial.print("\t");Serial.print(xavg);Serial.print("\t");Serial.print(yavg);Serial.print("\t");Serial.print(zavg);Serial.println("\t");
          Serial.println("-----------------------------------------");
        */
        Serial.print("Angle");Serial.print("\t");Serial.println(xavg); 
        Serial.println("-----------------------------------------");
        
        delay(500);
        i=-1; //reset counter 
      }
    
    //Printing value differences from 2MPUs
    /*  Serial.println("-----------------------------------------");
      Serial.print("Angle X= "); Serial.print(xval); Serial.print("\t");
      Serial.print("Y= "); Serial.print(yval); Serial.print("\t");
      Serial.print("Z= "); Serial.println(zval); 
    */  
    //Print line
      //Serial.println("-----------------------------------------");
      //delay(500);
i++;
}


  /*
 
  //Print X Values
  Serial.print("X= "); Serial.print(x); Serial.print("\t"); Serial.print("X2= "); Serial.println(x2);
  
  //Print Y Values
  Serial.print("Y= "); Serial.print(y); Serial.print("\t"); Serial.print("Y2= "); Serial.println(y2);
  
  //Print Z Values
  Serial.print("Z= "); Serial.print(z); Serial.print("\t"); Serial.print("Z2= "); Serial.println(z2);

  //Print Line
  Serial.println("-----------------------------------------");
  delay(500);
  */




























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

