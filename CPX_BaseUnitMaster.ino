
//Thermal Definitions\
/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 12 
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
bool InitialTemp = false;
float TargetTemp = 75; //Deg F
int TempRelay = 5;

//Motor Definitions\
/********************************************************************/
// connect motor controller pins to Arduino digital pins
// motor one
int enA = 11;
int in1 = 10;
int in2 = 9;
int xSpeed = 255; //global max motor speed, 8bit
int xTime = 500; //decel time, ms
/********************************************************************/

//Pump Definitions\
/********************************************************************/
int pumpRelay = 6; //data signal to the relay


//Helper Functions - By operation Type\
/++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//Temp Functions\
/********************************************************************/
float getTemp(){
  float T1;
  float T2;
  float T3;
  float Temp;
  sensors.requestTemperatures();
  T1 = sensors.getTempCByIndex(0)*9/5+32;
  T2 = sensors.getTempCByIndex(1)*9/5+32;
  T3 = sensors.getTempCByIndex(2)*9/5+32;
  Temp = (T1+T2+T3)-max(max(T1,T2),T3)-min(min(T1,T2),T3); //median
    return Temp;
}

void heatOnOff(){
  float Temp;
  Temp = getTemp();
//if in range or below,heat
  if(Temp<TargetTemp-.2)
  {
   digitalWrite(TempRelay, HIGH); //power to the heating element
  }
else{
  //FailSafe OFF - Seems to work better due to latency of the heat going from the element to the upper pool
  digitalWrite(TempRelay, LOW);
}

  
}


//Motor Functions\
/********************************************************************/
void decel()
{
  int x;
  int s;
  int i;
 // this function will decelerate the motor 75% of its speed
 // decelerate from maximum speed to zero
for (int i = xSpeed; i > 0; i = i-.35*xSpeed)
  {
    analogWrite(enA, i);
    delay(100);
    Serial.println(i);
  }
}

void decelRev()
{
  int x;
  int s;
  int i;
 // this function will decelerate the motor 75% of its speed
 // decelerate from maximum speed to zero
for (int i = xSpeed; i > 0; i = i-.35*xSpeed)
  {
    analogWrite(enA, i);
    delay(10);
    Serial.println(i);
  }
}

void driveMotorForward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA, xSpeed);
}

void driveMotorReverse(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(enA, xSpeed);
}
void driveMotorFlip(){
  delay(3000);
  decel();
  //Switch Direction
  driveMotorReverse();
  Serial.println("REVERSE");
  delay(3000);
  //Slow Down
  decel();
  Serial.println("FORWARD");
}



void setup() {
  Serial.begin(9600);
//Thermal Setup\
//********************************************************************/
 pinMode(TempRelay, OUTPUT);
  digitalWrite(TempRelay, LOW);
//********************************************************************/

//Motor Setup\
//********************************************************************/
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

//Pump Setup\
//********************************************************************/
  
  pinMode(pumpRelay, OUTPUT);
  digitalWrite(pumpRelay, HIGH); //Start the circulation pump
}

void loop() {
  // Start by heating the water
  float Temp = getTemp();

  // During Initial heating, if the water temperature is below target range, 
  //turn it on and leave it on
 // We only want this code to run once, so toggle the InitialTemp boolean
 //when done
 if(!InitialTemp){
  while(Temp <TargetTemp-.2){
      if(digitalRead(TempRelay)==LOW){
        digitalWrite(TempRelay, HIGH);
      }
      InitialTemp = true;
      Temp = getTemp();
      //Serial.print("Deg F = ");
   //Serial.println(Temp);
   delay(500);
    }
 }
     //Start Forward
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA, xSpeed);
  heatOnOff();
  delay(3000);
  //Slow Down
  decel();
  //Switch Direction
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(enA,xSpeed);
  delay(3000);
  //Slow Down
  decelRev();
  //Water now at temperature and we're ready to spin
 // driveMotorForward(); //Start motor
  //heatOnOff();        //Check the temp while we're just running at constant velocity
//  decel();
//  driveMotorReverse();
//  heatOnOff(); 
//  decel();
  //Slow Down
  //Serial.println(Temp);
  //driveMotorFlip();   //Reverse
  
}
