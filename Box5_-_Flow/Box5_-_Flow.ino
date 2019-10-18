
#include<Wire.h>
#define FLOW2SEC 100 //milliliters/second
#define MASTEROK 13

int inputVar = 0;

//IO Declaration
int washGo = 4;

int washRelay = 7;
int airRelay = 8;

int flowIn = 10;
int flowOut = 11;

//State Declarations
int washGoState = 0;

int fluidVolume = 0;
float fluidTime = 0; //seconds


void setup() {
  Serial.begin(9600);
  Wire.begin(5); // I2C assignment
  Wire.onReceive(receiveEvent);

  //Pin Modes


}

void receiveEvent () //Master has told this unit to do something
{
  digitalWrite(MASTEROK,LOW);
fluidVolume = Wire.read();
fluidTime = fluidVolume*FLOW2SEC;

MonitorForFlow();//SubFuntion for waiting for rinse or flow
}



void loop() {
delay(100);
}

void MonitorForFlow(){
  //initial Read States


  while(true){
    digitalWrite(MASTEROK,LOW);//reset from previous run
    washStartState = digitalRead(washStart);
    flowStartState = digitalRead(flowStart);

    if(washGoState == HIGH){
      //Turn on the rinse Solenoid
      digitalWrite(washRelay, HIGH);
      while(digitalRead(flowIn)==LOW){
        delay(100);
      }
      //Flow has now been detected. Wait for flow to occur
      delay(fluidTime);
      //Fluid is now in JOBO, turn off Solenoid
      digitalWrite(washRelay,LOW);
      //signalReady
      digitalWrite(MASTEROK,HIGH);
    }
    else{
      //Turn on the rinse Solenoid
      digitalWrite(airRelay, HIGH);
      while(digitalRead(flowIn)==LOW){
        delay(100);
      }
      //Flow has now been detected. Wait for flow to occur
      delay(fluidTime);
      //Fluid is now in JOBO, turn off Solenoid
      digitalWrite(airRelay,LOW);
      //signalReady
      digitalWrite(MASTEROK,HIGH);
      }
      //delay slightly before turning the OK switch MonitorForFlow
      delay(5000);
}
