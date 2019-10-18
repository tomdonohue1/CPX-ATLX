//-------------------------------------------------------------------------
//-------------------------Box 0 - Master Control---------------------
//-------------------------------------------------------------------------
//This interfaces directly with both a computer and the 4 boxes
//
//-------------------------------------------------------------------------
#include<Wire.h>
float Temp = 0.0; //Target temperature
char ProcessType = 'x';//C41, E6, B&W
char confirm = 'x';
int fluidVolume = 0;
bool processConfirm = false;
//I/O Declaration
int box1Ready = 11;
int box2Ready = 12;

void setup() {
Serial.begin(9600);
Wire.begin();

//I/O setup
 //Box 1 ready
 pinMode(box1Ready, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
//Welcom Message
Serial.println("---------------------------");
Serial.println("Welcome to the JOBO ATL-X");
Serial.println("---------------------------");

Serial.println("Select Process Type");
Serial.println("Press C for C41");
Serial.println("Press E for E6");
Serial.println("Press B for B&W");

while(!processConfirm){
  if(Serial.available()>0){
    ProcessType = Serial.read();
    Serial.println("Confirm Process:");
    switch (ProcessType) {
      case 'C':
      Serial.println("C-41 - Press y to confirm");
      if(Serial.available()>0){
        confirm = Serial.read();
      }
      if(confirm!='y'){
        Serial.println("Resetting...");
      }
      else{
        processConfirm = true;
        C41();
      }
      break;

      case 'E':
      Serial.println("E-6 - Press y to confirm");
      if(Serial.available()>0){
        confirm = Serial.read();
      }
      if(confirm!='y'){
        Serial.println("Resetting...");
      }
      else{
        processConfirm = true;
        E6();
      }
      break;

      case 'B':
      Serial.println("Black and White - Press y to confirm");
      if(Serial.available()>0){
        confirm = Serial.read();
      }
      if(confirm!="y"){
        Serial.println("Resetting...");
      }
      else{
        processConfirm = true;
        BandW();
      }
      break;
    }
  }
}
}
//------------------------------------------------------------------
//----Process Functions---------------------------------------------

//C-41 Color Negative
//No varaiation assumed - Fresh chems all the Time
void C41(){
  processConfirm = false;
  confirm = "n";
  Serial.println("C-41 Color Negative Process");

  //Get Fluid Volume and Confirm
  while(!processConfirm){
    Serial.println("Enter Fluid Volume");
    if(Serial.available()>0){
      fluidVolume = Serial.read();
    }
    Serial.print("Confirm Fluid Volume:");
    Serial.print(fluidVolume);
    Serial.print("ml");
    if(Serial.available()>0){
      confirm = Serial.read();
    }
    if(confirm!="y"){
      Serial.println("retry");
    }
    else{
      processConfirm = true;
    }
  }

//C-41 is Temperature Controlled. Bath will be needed.
//Fill The BathOverflow
Wire.beginTransmission(1);
Wire.write(1); //1 is the switch case for filling
Wire.endTransmission();
//While filling starts, send the target temp to Box 2
Wire.beginTransmission(2);
Wire.write(38); //38°C to box 2
Wire.endTransmission();

//wait for both the bath to fill and for everything to come to Temperature
while(digitalRead(box1Ready)==LOW && digitalRead(box2Ready)==LOW){
  delay(10000); //wait 10 seconds at a time
}
//Bath is Filled, we can start with the Developer






//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //END C-41
}

void E6(){
  
}

void BandW(){
  
}
