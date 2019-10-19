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
int box3Ready = 13;
int box4Ready = 14;
int box5Ready = 15;

//OUTPUT
int recGo = 20; //Toggle to rinse/pneum channel Box 3
//High - Reclamation Channel, Low - Pneum Channel



void setup() {
Serial.begin(9600);
Wire.begin();

//I/O setup
 //Box 1 ready
 pinMode(box1Ready, INPUT);
 pinMode(box2Ready, INPUT);
 pinMode(box3Ready, INPUT);
 pinMode(box4Ready, INPUT);
 pinMode(box5Ready, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
//Welcom Message
Serial.println("---------------------------");
Serial.println("Welcome to the JERB-O ATL-X");
Serial.println("---------------------------");

Serial.println("Select Process Type");
Serial.println("Press c for C41");
Serial.println("Press e for E6");
Serial.println("Press b for B&W");
while(Serial.available() == 0){ }
if(Serial.available()>0){
    ProcessType = Serial.read();
  }
while(!processConfirm){
    Serial.println("Confirm Process:");

    switch (ProcessType) {
      case 'C':
      Serial.println("C-41 - Press y to confirm");
      while(Serial.available() == 0){ }
      if(Serial.available()>0){
        confirm = Serial.read();
        Serial.println(confirm);
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
      while(Serial.available() == 0){ }
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
      while(Serial.available() == 0){ }
      if(Serial.available()>0){
        confirm = Serial.read();
      }
      if(confirm!='y'){
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

//------------------------------------------------------------------
//----Process Functions---------------------------------------------

//C-41 Color Negative
//No varaiation assumed - Fresh chems all the Time
void C41(){

  long timeDev = 3.25*60*1000L; //3:15 to seconds
  long timeBle = 6.5*60*1000L; //6:30 to Seconds
  long timeFix = 6.5*60*1000L; //6:30 to Seconds

  processConfirm = false;
  confirm = 'n';
  Serial.println("C-41 Color Negative Process");

  //Get Fluid Volume and Confirm
  while(!processConfirm){
    Serial.println("Enter Fluid Volume");
    while(Serial.available() == 0){ }
    if(Serial.available()>0){
      fluidVolume = Serial.parseInt();
    }
    Serial.print("Confirm Fluid Volume: ");
    Serial.print(fluidVolume);
    Serial.print(" ml");
    while(Serial.available() == 0){ }
    if(Serial.available()>0){
      confirm = Serial.read();
    }
    if(confirm!='y'){
      Serial.println("retry");
    }
    else{
      processConfirm = true;
    }
  }

//C-41 is Temperature Controlled. Bath will be needed.
//Fill The BathOverflow
Serial.println("Fill Command Sent");
SendCommand(1,1);

//While filling starts, send the target temp to Box 2
Serial.println("Target Temp Sent to Controller");
SendCommand(2,380); //Temperature is sent as Temp*10 so that a decimal can be used while still only sending an int data type.
//Tell the Flow box how much water to used
Serial.println("Fluid Volume Sent to Controller");
SendCommand(5,fluidVolume);
//Set the input Channel on the Pneumatic Roller while we're waiting

//Set the input Channel on the Reclamation
Serial.println("Set Reclamation Channel W");
SendCommand(3,10);
//Don't need to wait for the box because heating will take longer
//wait for both the bath to fill and for everything to come to Temperature
Serial.println("Bath Heating, Please Wait....");
Serial.print("Box 1 Status: ");
Serial.println(digitalRead(box1Ready)==LOW);
Serial.print("Box 2 Status: ");
Serial.println(digitalRead(box2Ready)==LOW);
Serial.print("Box 5 Status: ");
Serial.println(digitalRead(box5Ready)==LOW);
while(digitalRead(box1Ready)==LOW && digitalRead(box2Ready)==LOW && digitalRead(box5Ready) == LOW){
  delay(5000);
  Serial.print("."); //wait 5 seconds at a time
}
//Bath is Filled, we can start with the inital Rinse
Serial.println("Heating Complete! Begining Initial Rinse");
//Start Spinning
SendCommand(4,1);
//rinse
Rinse(30);
//Step One - Developer
Serial.println("Begin Chemical 1");
ProcessChem(1,timeDev);
//Chems Dumped, Rinse Thrice
Serial.println("Chemical Complete, in rinse cycle");
Rinse(30);
Rinse(30);
Rinse(30);
//Step Two - Bleach
Serial.println("Begin Chemical 2");
ProcessChem(2,timeBle);
//Chems Dumped, Rinse Thrice
Serial.println("Chemical Complete, in rinse cycle");
Rinse(30);
Rinse(30);
Rinse(30);
//Step 3 - Fix
Serial.println("Begin Chemical 3");
ProcessChem(3,timeFix);
//Chems Dumped, Rinse Thrice
Serial.println("Chemical Complete, in rinse cycle");
Rinse(30);
Rinse(30);
Rinse(30);

Serial.println("Process Complete!");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //END C-41
}

void SendCommand(int Box, int Command){
  Wire.beginTransmission(Box);
  Wire.write(Command);
  Wire.endTransmission();
}

void Wait4Box(int Box){
  while(digitalRead(Box)==LOW){
    delay(50);
  }
}
void Rinse(int rinseTime){
  //parses an int for total rinse timer
  rinseTime = rinseTime*1000; //convert Seconds to Milliseconds
  SendCommand(5,1);//Fill Jobo with Water
  Wait4Box(box5Ready);
  //Fluid is now in JOBO, run the timer
  delay(rinseTime); //60 second rinse
  //Times up! Dump Rinse Water
  SendCommand(4,2);
  Wait4Box(box4Ready);
}

void ProcessChem(int channel,int time){
  //time input in ms
  //Start by setting the PneumaticRoller
  SendCommand(3,channel+10);//+10 for Pnumatic Roller
  Wait4Box(box3Ready);
  //Set the Reclamation channel

  SendCommand(3,channel+20);//+20 for Reclamation
    //Waiting not needed as this will take less than process times

  SendCommand(5,2);//Tell fluid to flowIn
  Wait4Box(box5Ready); //fluid is now fully in Jobo
  delay(time);
  SendCommand(4,2);//dump chemicals
  Wait4Box(box4Ready);
}
void E6(){
Serial.println("E-6 not currently supported");
}

void BandW(){
Serial.println("Black and White not currently supported");
}
