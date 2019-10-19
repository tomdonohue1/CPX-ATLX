
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

//Other Declaration

void setup() {
  Serial.begin(9600);
  Wire.begin(3); // I2C assignment
  Wire.onReceive(receiveEvent);

//PinModes
pinMode(MASTEROK, OUTPUT);
pinMode(PULp,OUTPUT);
pinMode(DIRp,OUTPUT);
pinMode(ENAp,OUTPUT);
pinMode(hallp,OUTPUT);

pinMode(PULr,OUTPUT);
pinMode(DIRr,OUTPUT);
pinMode(ENAr,OUTPUT);
pinMode(hallr,OUTPUT);
}

void receiveEvent () //Master has told this unit to do something
{
  digitalWrite(MASTEROK,LOW);//Reset the OK switch
  int channelTarget = 0; //reset target channel
  inputVar = Wire.read();

  if(inputVar>10 && inputVar<20){ //Pnumatic Channel Set
    channelTarget = channelTarget-10; //use only the last digit
    zeroRoller();
    PneuChannelSet(channelTarget);
    digitalWrite(MASTEROK,HIGH);
  }
  else if(inputVar>=20 && inputVar <30){ //Reclamation Channel Set
    channelTarget = channelTarget-20; //use only the last digit
  //xxx Code for Rec. Channel Function Goes here
  digitalWrite(MASTEROK,HIGH);
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
  if(stsp == LOW){
    while (stsp == LOW){
      forward();
      stsp = digitalRead(hallp);
    }
  }
  stsp = digitalRead(hallp); // Current State
  //Advance Forward until we hit the magnet again
  while (stsp == HIGH){
    forward();
    stsp = digitalRead(hallp);

  }
  delay(100);
  //Now, count the steps until we're past the magnet
  while (stsp == LOW){
    forward();
    zeroSteps ++;
    stsp = digitalRead(hallp);
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
