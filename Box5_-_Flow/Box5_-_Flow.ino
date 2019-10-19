
#include<Wire.h>
#define FLOW2SEC 100 //milliliters/second
#define MASTEROK 13

int inputVar = 0;

//IO Declaration
int washGo = 4;
int airGo = 5;

int washRelay = 7;
int airRelay = 8;

int flowIn = 10;


//State Declarations
int washGoState = 0;
int airGoState = 0;
int fluidVolume = 0;
float fluidTime = 0; //seconds


void setup() {
  Serial.begin(9600);
  Wire.begin(5); // I2C assignment
  Wire.onReceive(receiveEvent);

  //Pin Modes
 pinMode(MASTEROK, OUTPUT);

}

void receiveEvent () //Master has told this unit to do something
{
  digitalWrite(MASTEROK,LOW);
inputVar = Wire.read();

if(inputVar>2){ //the fluid volume has been sent
  fluidVolume = Wire.read();
  fluidTime = fluidVolume*FLOW2SEC;
}
//else, we can just use inputVar as our switch variable
  switch (inputVar) {
    case 1://washGo
    digitalWrite(washRelay, HIGH);
    Wait4(flowIn);
    //Flow has now been detected. Wait for all water to enter JOBO
    delay(fluidTime*1.5);//Always Rinse with more water than fluid
    //Fluid is now in JOBO, turn off Solenoid
    digitalWrite(washRelay,LOW);
    //signalReady
    digitalWrite(MASTEROK,HIGH);
    break;

    case 2://airGo
    digitalWrite(airRelay, HIGH);
    Wait4(flowIn);
    //Flow has now been detected. Wait for fluid to enter JOBO
    delay(fluidTime);
    //Fluid is now in JOBO, turn off Solenoid
    digitalWrite(airRelay,LOW);
    //signalReady
    digitalWrite(MASTEROK,HIGH);
    break
    default:

    break;
  }


void loop() {
delay(100);
}

void Wait4(int wait){
  while(digitalRead(wait)==LOW){
    delay(50);
  }
}
