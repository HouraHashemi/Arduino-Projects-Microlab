/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>
#include <Keypad.h>

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
//------------------------------------------------------------

Servo myservo;  // create servo object to control a servo

String input = "0";
char key;
bool check;

void setup() {
  pinMode(23, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("ENTER ANGELE: (FOR CONFIRM PRESS \"c\" IN KEYPAD.)");
  myservo.attach(13);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
}

void loop() {
  // user enter key between 0 to 180
  key = keypad.getKey();
  if(key){
    if(key == 'c'){
      check = true;
    }
    else{
      Serial.print(key);
      input = input + key;
    }
  }
    
  if(check==true){
    if(input.toInt()>180 || input.toInt()<0){
      Serial.println("ERROR.\n");
    }
    else{
      Serial.println();
      Serial.println("SERVO IS RUNNING.");
//      int val = (input.toInt(),0,180,-90,90);
//      myservo.write(val);
      myservo.write(input.toInt());
      delay(500);
      Serial.println("--------------------------");
    }
    check = false;
    input ="0";
  }
  
}
