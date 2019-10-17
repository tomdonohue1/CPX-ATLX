//-------------------------------------------------------------------------
//-------------------------Box 0 - Master Control---------------------
//-------------------------------------------------------------------------
//This interfaces directly with both a computer and the 4 boxes
//
//-------------------------------------------------------------------------
#include<Wire.h>
float Temp = 0.0; //Target temperature
char ProcessType = 'x';//C41, E6, B&W

void setup() {
Serial.begin(9600);
Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//Welcom Message
Serial.println("---------------------------");
Serial.println("Welcome to the JOBO ATL-X");
Serial.println("---------------------------");
}
