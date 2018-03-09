#include <SoftwareSerial.h>

SoftwareSerial xbee(8, 9);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);

}

char imus[] = {'a','b','c'};
long iteration;
void loop() {
  iteration++;
  for(int i = 0; i < sizeof(imus)/sizeof(char); i++){
    float angle = emulateIMU(iteration,i);
    Serial.println(String(angle,3) + imus[i]);
    xbee.print(String(angle,3) + imus[i]);
  }
  delay(100);
}

float angle = 30.0;
float emulateIMU(int iteration,int imu){
  return sin(iteration * 0.25 + imu) * angle;
}
  
