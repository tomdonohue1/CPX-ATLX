//-------------------------------------------------------------------------
//-------------------------Box 1 - Fill and Drain---------------------
//-------------------------------------------------------------------------
//This controller is for all functions related to the initial fill, waste 
//drainage, and final Drain.
//-------------------------------------------------------------------------
#include<Wire.h>  
int inputVar = 0;
//Global Variables

//Terminal Assignments
int inPump = 5;
int drainPump = 4;
int outPlug = 6;

int wasteFull = 10;
int wasteEmpty = 9;
int bathOverflow = 8;
int inBathFlow = 7;

int fillDone = 11;
int drainDone = 12;

//Monitor Bools
bool wasteIsFull = false;

void setup() {
  Serial.begin(9600); 
  Wire.begin(1); // I2C assignment
  Wire.onReceive(receiveEvent);

  pinMode(inPump,OUTPUT);
  pinMode(drainPump,OUTPUT);
  pinMode(outPlug,OUTPUT);

  pinMode(wasteFull,INPUT);
  pinMode(wasteEmpty,INPUT);
  pinMode(bathOverflow,INPUT);
  pinMode(inBathFlow,INPUT);

  pinMode(fillDone,OUTPUT);
  pinMode(drainDone,OUTPUT);
}

void receiveEvent () //Master has told this unit to do something
{
 inputVar = Wire.read(); 

        switch(inputVar){
          case 1: //fill bath
            digitalWrite(inPump,HIGH);
            if(digitalRead(inBathFlow)==HIGH){
              while(digitalRead(inBathFlow) ==HIGH && digitalRead(bathOverflow == HIGH))
              delay(100);
            }
            else{
              delay(100);
            }
            digitalWrite(fillDone,HIGH);//signal bath is filled
            
          break;
          case 2: //Monitor
          while(true){
            if(digitalRead(wasteFull) == HIGH){ //need to drain
              digitalWrite(drainPump, HIGH);
              while(digitalRead(wasteEmpty == HIGH)){
                delay(100);
              }
              //water is below min, turn off pump
              digitalWrite(drainPump,LOW);
            }
          }
          break;
          case 3: //Drain
          digitalWrite(outPlug, HIGH);
          delay(300000); //wait 5 minutes to drain
          digitalWrite(outPlug, LOW); //turn the drain relay off
          break;
          default:

          break;
        }
}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);
}
