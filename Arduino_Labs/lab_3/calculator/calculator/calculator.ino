#include <LiquidCrystal.h>
#include <Keypad.h>

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
//----------------------------------------------------

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'c','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//-----------------------------------------------------
char key;
char operand;
String temp="",input1="",input2="";
int result = 0;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  PROGRAMED BY:  ");
  lcd.setCursor(0,1);
  lcd.print("   NOURA.S.H   ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-- CALCULATOR --");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  // put your setup code here, to run once:
}


void loop() {
  
  key = keypad.getKey();
  if(key){
    if(key=='c'){
      clear_data(temp, input1, input2, operand);
     }
    else if(key=='*' || key=='/' || key=='+' ||key=='-'){
      lcd.print(key);
      operand = key;
      input1 = temp;
      temp ="";
    }
    else if(key == '='){
        input2 = temp;
        lcd.setCursor(0,1);
        lcd.print("=");
        if(input1=="" || input2==""){
          lcd.print("ERROR");
        }
        else{
          switch (operand){
            case '/':
              lcd.print(input1.toInt()/input2.toInt());
              break;
            case '*':
              lcd.print(input1.toInt()*input2.toInt());
              break;
            case '-':
              lcd.print(input1.toInt()-input2.toInt());
              break;
            case '+':
              lcd.print(input1.toInt()+input2.toInt());
              break;
            default:
              lcd.print("ERROR");
              break;
          }
        }
        while(keypad.waitForKey()!='c');
        clear_data(temp, input1, input2, operand);
     }
     else{
        temp = temp + key;
        lcd.print(key);
     }
     
   }//end getkey
// put your main code here, to run repeatedly:
}

//reset all data and clean lcd
void clear_data(String &t, String &in1, String &in2, char op){
  lcd.clear();
  lcd.setCursor(0,0);
  t = "";
  in1 = "";
  in2 = "";
}
