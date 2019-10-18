
#include<Wire.h>
#define MASTEROK 13

int inputVar = 0;

//Pneumatic Declaration
int PULp=8; //define Pulse pin
int DIRp=9; //define Direction pin
int ENAp=10; //define Enable Pin

int hallp = 11; //input
int stsp = 0; //limit switch status

//Reclamation Declaration
int PULr=5; //define Pulse pin
int DIRr=6; //define Direction pin
int ENAr=7; //define Enable Pin

int hallr = 4; //input
int stsr = 0; //limit switch status
int recGo = 3; //if HIGH, set Rec Channel - if LOW, set Pneum Channel
int recGoSts = 0;
//Other Declaration

void setup() {
  Serial.begin(9600);
  Wire.begin(3); // I2C assignment
  Wire.onReceive(receiveEvent);


//PinModes

}

void receiveEvent () //Master has told this unit to do something
{
  digitalWrite(MASTEROK,LOW);//Reset the OK switch
inputVar = Wire.read();
recGoSts = digitalRead(recGo);

if(recGoSts == LOW){}
    zeroRoller();
    PneuChannelSet(subChannel);
    digitalWrite(MASTEROK,HIGH);
  }
else{
  //xxx Code for Rec. Channel Function Goes here
  digitalWrite(MASTEROK,HIGH);
}
    subChannel = inputVar[1];
    //SubFuctions to be done
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
