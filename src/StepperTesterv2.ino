int PUL=6; //define Pulse pin
int DIR=5; //define Direction pin
int ENA=4; //define Enable Pin

int hall = 7; //input
int sts = 0; //limit switch status

  int stepsStart = 0;
  int stepsDone = 0;
  int start = 0;
void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (hall, INPUT);
  Serial.begin(9600);


}

void forward(){
    digitalWrite(DIR,LOW);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(5);
    digitalWrite(PUL,LOW);
    delayMicroseconds(5);
}

void backward(){
    digitalWrite(DIR,HIGH);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(5);
    digitalWrite(PUL,LOW);
    delayMicroseconds(5);
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

void ChannelSet(long channel){
  long numSets = 80000*channel;
  for(long i = 0; i<numSets; i++)
  forward();
  Serial.println("Channel Set");
}

void loop() {
  char channelChar[] = "0";
  long channel = 0;
 Serial.println("BEGIN ");
 zeroRoller();
// Serial.println("Select Channel:");
//  // send data only when you receive data:
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    channel = Serial.read();
//  }
//  channel = channelChar.toInt()
//  Serial.print("Chosen channel: ");
//  Serial.println(channel);
ChannelSet(3);

delay(99999);
}
