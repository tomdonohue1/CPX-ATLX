
#include<Wire.h>

str inputVar = String(2);
char subSystem = 'x';
int subChannel = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(3); // I2C assignment
  Wire.onReceive(receiveEvent);

}

void receiveEvent () //Master has told this unit to do something
{
inputVar = Wire.read();
subSystem = inputVar[0];
subChannel = inputVar[1];



}



void loop() {
delay(100);

}
