
#include<Wire.h>

#define MASTEROK 13

int inputVar = 0;
int dwVal = 0; //Read store int
int upVal = 0;

//I/O Declaration
int upLimit = 4;
int dwLimit = 5;
int flowOut = 11;

//Lift Motor Declaration
int enA_L = 910;
int in1_L = 3;
int in2_L = 2;
int xSpeed = 128; //global max motor speed, 8bit

//Spin Motor Delcaration
int enA_S = 9;
int in1_S = 3;
int in2_S = 2;
int xSpeed_S = 255;


void setup() {
  Serial.begin(9600);
  Wire.begin(4); // I2C assignment
  Wire.onReceive(receiveEvent);

  //Pin Modes
pinMode(MASTEROK, OUTPUT);
  pinMode(upLimit,INPUT);
  pinMode(dwLimit,INPUT);
  pinMode(flowOut,INPUT);

  pinMode(enA_L,OUTPUT);
  pinMode(in1_L,OUTPUT);
  pinMode(in2_L,OUTPUT);

  pinMode(enA_S,OUTPUT);
  pinMode(in1_S,OUTPUT);
  pinMode(in2_S,OUTPUT);
}

void receiveEvent () //Master has told this unit to do something
{
  digitalWrite(MASTEROK,LOW);//reset OK

  inputVar = Wire.read();

  switch (inputVar) {
    case 1://Spin
      spin();
    break;
    case 2: //Cycle Lift
    upward();
    //Wait for out flow
    while(digitalRead(flowOut)==LOW){
      delay(50);
    }
    //flow is happening
    while(digitalRead(flowOut)==HIGH){
      delay(100);
    }
    //flow is done
    delay(5000); //delay +5 seconds just to be sure
    downward();
    digitalWrite(MASTEROK,HIGH)
    spin();//assuming that the receive event will reset spin
    break;
  }
}

//Lift Motor functions
//------------------------------------------------------------------
void upward(){
  if(digitalRead(upLift)==HIGH){
    upVal = digitalRead(upLift); //Read Pin
    digitalWrite(in1_L,HIGH);
    digitalWrite(in2_L,LOW);
    analogWrite(enA_L, xSpeed);
    while((upVal == HIGH)){
      delay(50);
      upVal = digitalRead(upLift); //Read Pin
  }
  }
  else{
    Serial.println("already up");
  }
  pause();
}

void pause(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
}

void downward(){
  Serial.println("Going Down");
  if(digitalRead(dwLift)==HIGH){
    dwVal = digitalRead(dwLift); //Read Pin
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(enA, xSpeed);
    while((dwVal == HIGH)){
      delay(50);
      dwVal = digitalRead(dwLift); //Read Pin
  }
  }
  else{
    Serial.println("already down");
  }
  pause();
}

//Spin Motor functions
//------------------------------------------------------------------
void spin(){
while(true){
digitalWrite(in1_S,HIGH);
digitalWrite(in2_S,LOW);
analogWrite(enA_S, xSpeed_S);
heatOnOff();
delay(3000);
//Slow Down
decel();
//Switch Direction
digitalWrite(in1_S,LOW);
digitalWrite(in2_S,HIGH);
analogWrite(enA_S,xSpeed);
delay(3000);
//Slow Down
decelRev();
}
}
void decel()
{
  int x;
  int i;
 // this function will decelerate the motor 75% of its speed
 // decelerate from maximum speed to zero
for (int i = xSpeed_S; i > 0; i = i-.35*xSpeed_S)
  {
    analogWrite(enA_S, i);
    delay(100);
  }
}

void decelRev()
{
  int x;
  int i;
 // this function will decelerate the motor 75% of its speed
 // decelerate from maximum speed to zero
for (int i = xSpeed_S; i > 0; i = i-.35*xSpeed_S)
  {
    analogWrite(enA_S, i);
    delay(10);
  }
}
