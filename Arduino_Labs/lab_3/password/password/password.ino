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

const String my_name = "NOURA";
const String password = "1234";
char key;
String input;
char counter = 0;
String data[6][1] ={{" "},
                    {" 3         3     3333      3333"},
                    {" 3        3 3    3   3        3"},
                    {" 3       33333   3333      3333"},
                    {" 3      3     3  3   3        3"},
                    {" 33333 3       3 3333 ____ 3333"}
                    };

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
  // put your setup code here, to run once:
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("*ENTER PASSWORD:");
  key = keypad.getKey();
  if(key){
    input = input + key;
    lcd.setCursor(counter,1);
    lcd.print(key);
    counter=counter+1;
    if(counter==4){ 
          delay(200);
          lcd.clear();
          lcd.setCursor(0,0);
         if(input == password){ 
          lcd.print("CORRECT PASSWORD");
          for(int i=0; i<6; i++){
            Serial.println(data[i][0]);
          }
          delay(2000);
         }else{
          lcd.print("WORONG  PASSWORD");
          delay(2000);
         }
         input = "";
         lcd.clear();
         counter = 0;
    }
  }
  // put your main code here, to run repeatedly:

}
