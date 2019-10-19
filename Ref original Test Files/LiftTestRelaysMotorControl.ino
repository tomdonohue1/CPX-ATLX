// connect motor controller pins to Arduino digital pins
// motor one
int enA = 11;
int in1 = 10;
int in2 = 9;
int xSpeed = 60; //global max motor speed, 8bit
int xTime = 500; //decel time, ms
int i = 0;

int upLift = 8; //define Hall input for up position
int dwLift = 7; //define Hall input for down position

int dwVal = 0; //Read store int
int upVal = 0;

char motion = "k";
void setup()
{
  Serial.begin(9600);
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(upLift, INPUT);
  pinMode(dwLift, INPUT);

}


void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    motion = Serial.read();
  Serial.println("command received");
  Serial.println(motion);
  Serial.println(digitalRead(upLift));
    if(motion == 'u'){
      upward();
    }
    else if(motion == 'd'){
      downward();
    }
    else{
      Serial.println("Invalid input");
    }
  }
}

void upward(){
  Serial.println("Going up");
  if(digitalRead(upLift)==HIGH){
    upVal = digitalRead(upLift); //Read Pin
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(enA, xSpeed);
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
