//-------------------------------------------------------------------------
//-------------------------Box 2 - Temp Bath Control---------------------
//-------------------------------------------------------------------------
//This controller is for all functions related to the temperature bath
//This will fill the bath when asked, then set and maintain a temperature
//given to it by the master arduino
//-------------------------------------------------------------------------
#include<Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
int inputVar = 0; //goal Temp
//Global Variables
int circPump = 3;
int heater = 4;
int bathTemps = 8;
int chemTemps = 7;
int fillReady = 6;

bool initTempGood = false;

//output
int tempReady = 12;
OneWire bathOneWire(bathTemps);
OneWire chemOneWire(chemTemps);

DallasTemperature bathWire(&bathOneWire);
DallasTemperature chemWire(&chemOneWire);

void setup() {
  Serial.begin(9600);
  Wire.begin(2); // I2C assignment
  Wire.onReceive(receiveEvent);
}

void receiveEvent () //Master has told this unit to do something
{
 inputVar = Wire.read(); // get target temp
 inputVar = (float)inputVar; // convert to a float for exact control
 //Wait for the bath to fill, then start the circ. pump
 while(digitalRead(fillReady) == false){
  delay(1000);
 }
 //turn on circ pump
 digitalWrite(circPump, HIGH); //start circPump
 TempControlBath(inputVar);

}

void TempControlBath(float TargetTemp){
  float Temp;
  Temp = getTempChem();
//if in range or below,heat

  while(true){
  if(Temp<TargetTemp-.2)
  {
   digitalWrite(heater, HIGH); //power to the heating element
  }
  else{
    digitalWrite(heater, LOW); //FailSafe OFF - Seems to work better due to latency of the heat going from the element to the upper pool
  }
  if(getTempChem()>=TargetTemp-.2){
    initTempGood = true;
    digitalWrite(tempReady,HIGH);//tell the master we're heated and ready
  }
 }
}
float getTempBath(){
  float T1;
  float T2;
  float T3;
  float Temp;
  bathWire.requestTemperatures();
  T1 = bathWire.getTempCByIndex(0)*9/5+32;
  T2 = bathWire.getTempCByIndex(1)*9/5+32;
  T3 = bathWire.getTempCByIndex(2)*9/5+32;
  Temp = (T1+T2+T3)-max(max(T1,T2),T3)-min(min(T1,T2),T3); //median
    return Temp;
}

float getTempChem(){
  float T1;
  float T2;
  float T3;
  float T4;
  float T5;
  float Temp;
  chemWire.requestTemperatures();
  T1 = chemWire.getTempCByIndex(0)*9/5+32;
  T2 = chemWire.getTempCByIndex(1)*9/5+32;
  T3 = chemWire.getTempCByIndex(2)*9/5+32;
  T4 = chemWire.getTempCByIndex(3)*9/5+32;
  T5 = chemWire.getTempCByIndex(4)*9/5+32;
  Temp = ((T1+T2+T3+T4+T5)-max(max(T1,T2),T3)-min(min(T1,T2),T3))/3; //quasi-median
    return Temp;
}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);
}
