/****************************************************************************
*********
  Mark Bramwell, July 2010
  This program will test the LCD panel and the buttons.When you push the
  button on the shield，
  the screen will show the corresponding one.
  Connection: Plug the LCD Keypad to the UNO(or other controllers)
*****************************************************************************
*********/
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
// define some values used by the panel and buttons
int lcd_key = 0;
int adc_key_in = 0;
String welcomeLine = "Jobo, but better";
String programs[3] = {"C41", "E6", "Mono"};
String yesNo[2] = {"Yes","No"};
int yesNoPos = 1; // initial state as no to prevent accitentail stuff.
int programsPos = 1; //Initial position of programs array is C41
int programSelect = 0; // Program to use
int programConfirm = 0;
int yesNoConfirm = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
int read_LCD_buttons() { // read the buttons
  int keySelection = 0;
  int currentState = 0;
  while (true) {
    currentState = analogRead(0);
    if (currentState > 1000) {}
    else {
      keySelection = currentState;
      break;
    }
    delay(50);
  }

  // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 50741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold

  if (keySelection < 50) return btnRIGHT;
  if (keySelection < 250) return btnUP;
  if (keySelection < 450) return btnDOWN;
  if (keySelection < 650) return btnLEFT;
  if (keySelection < 850) return btnSELECT;

  return btnNONE; // when all others fail, return this.
}
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // start the library
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  delay(2000);
  lcd.clear();
  lcd.setCursor(4, 0); // set the LCD cursor position
  lcd.print("ATL - X"); // print a simple message on the LCD
  lcd.setCursor(0, 1);
}
void loop() {

int yesNoPos = 1; // initial state as no to prevent accitentail stuff.
int programsPos = 1; //Initial position of programs array is C41
int programSelect = 0; // Program to use
int programConfirm = 0;
int yesNoConfirm = 0;



  //ScrollBottomLeft(welcomeLine);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Program");
  lcd.setCursor(0, 1);


  while (programSelect == 0) {
    lcd_key = read_LCD_buttons();
    //Serial.println(lcd_key);
    switch (lcd_key) {
      case btnUP: 
          if (programsPos != 2) {
            programsPos = ++programsPos;
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print(programs[programsPos]);
          }
       break;
        
      case btnDOWN: 
          if (programsPos != 0) {
            programsPos = --programsPos;
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print(programs[programsPos]);
          }
      break;

      case btnSELECT: 
          programSelect = programsPos+10;
      break;
        
    }
    delay(250);
  }

lcd.clear();
lcd.setCursor(0, 0);
  lcd.print("Selected Program");
  lcd.setCursor(0, 1);
  lcd.print(programs[programSelect-10]);
  lcd.print("   Y/N?");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(programs[programSelect-10]);
  lcd.print("   Y/N?");
  lcd.setCursor(0,1);
  lcd.print("No   Yes");
  


  while (yesNoConfirm == 0) {
    lcd_key = read_LCD_buttons();
    Serial.println(yesNoPos);
    switch (lcd_key) {
      case btnLEFT: //no
          yesNoConfirm = 11;
       break;
        
      case btnRIGHT: //yes
          yesNoConfirm = 10;
      break;

      case btnSELECT: 
          yesNoConfirm = yesNoConfirm;
      break;   
    }
  }

if(yesNoConfirm ==10){//yes confirmed
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Confirmed");
  delay(1000000000);
}

else{
  //nothing, jump back to the beginning
}

}

void ScrollBottomLeft(String inputString) {
  int lengthofString = inputString.length();

  for (int i = 0; i < lengthofString; i++) {
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 1);
    lcd.print(inputString.substring(i, lengthofString));
    delay(500);
  }
}
