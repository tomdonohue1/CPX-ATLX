
#include<Wire.h>
#define FLOW2SEC .1 //milliliters/second
#define MASTEROK A1

str inputVar = String(4);
char subSystem = 'x';
int subChannel = 0;

//Pneumatic Declaration
int PULp=8; //define Pulse pin
int DIRp=9; //define Direction pin
int ENAp=10; //define Enable Pin

int hallp = 4; //input
int stsp = 0; //limit switch status

//Reclamation Declaration
int PULr=5; //define Pulse pin
int DIRr=6; //define Direction pin
int ENAr=7; //define Enable Pin

int hallr = 4; //input
int stsr = 0; //limit switch status
//Other Declaration
int washRelay = 13;
int airRelay = 2;

int flowIn = 12;
int flowOut = 11;

int fluidVolume = 0;
float fluidTime = 0; //seconds


void setup() {
  Serial.begin(9600);
  Wire.begin(3); // I2C assignment
  Wire.onReceive(receiveEvent);

  //Pin Modes


}

void receiveEvent () //Master has told this unit to do something
{
inputVar = Wire.read();
subSystem = inputVar[0];


  switch (subSystem) {
    case 'f':
      fluidVolume = inputVar.substring(1,3);
      fluidTime = fluidVolume*FLOW2SEC*1000; //converts to miliseconds
    break;
    case 'p':
    subChannel = inputVar[1];
    zeroRoller();
    PneuChannelSet(subChannel);
    break;

    case 'r':
    subChannel = inputVar[1];
    //SubFuctions to be done
    break;

    case 'w': //rinse water
    //Turn on the rinse solenoid
    digitalWrite(washRelay, HIGH);
    while(digitalRead(flowIn) == LOW){
      delay(50);
    }
    //fluid is now flowing into the JOBO
    //Tell the Master to Start Spinning the wheel
    digitalWrite(MasterOK, HIGH);
    delay(fluidTime); //wait the required amount of time for proper fluid flowIn

    digitalWrite(washRelay, LOW);
    break;
  }

}



void loop() {
delay(100);

}


void zeroRoller(){
int zeroSteps = 0; //to be used counting steps of the magnet
int backSteps = 0; //to be used centering the magnet
  //First, make sure it's not on channel Zero
sts = digitalRead(hall); //LOW = MAGNET
  if(sts == LOW){
    Serial.println("In ON-MAGNET While Loop");
    while (sts == LOW){
      forward();
      sts = digitalRead(hall);
    }
  }
  sts = digitalRead(hall); // Current State
  //Advance Forward until we hit the magnet again
  Serial.println("In First While Loop");
  while (sts == HIGH){
    forward();
    sts = digitalRead(hall);

  }
  delay(500);
Serial.println("In Second While Loop");
  //Now, count the steps until we're past the magnet
  while (sts == LOW){
    forward();
    zeroSteps ++;
    sts = digitalRead(hall);
  }

  //Divide the number in half to center the motor
  backSteps = zeroSteps/2;
  //back up to Zero Position

  for (int i = 0; i<backSteps; i++){
    backward();
  }
}

void PneuChannelSet(long channel){
  long numSets = 80000*channel;
  for(long i = 0; i<numSets; i++)
  forward();
  Serial.println("Channel Set");
}
