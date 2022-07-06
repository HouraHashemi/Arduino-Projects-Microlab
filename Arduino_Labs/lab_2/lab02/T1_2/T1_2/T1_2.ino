/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
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
//char data = ' ';
//char led = ' ';
void setup(){
  Serial.begin(9600);
  keypad.setHoldTime(500);
  keypad.setDebounceTime(250);
//  pinMode(RX0,INPUT);
//  pinMode(TX0,INPUT);
  
  pinMode(23, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);
  
  pinMode(A1, OUTPUT);  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
}
  
void loop(){
  char led = keypad.getKey();
  
  if (led){
    Serial.println(led);
//    data = Serial.write(RX0);
//    led = Serial.read(data);
    if(led == '7'){
        digitalWrite(A7,HIGH);
        delay(500);
        digitalWrite(A7,LOW);
    }
    if(led == '8'){
        digitalWrite(A8,HIGH);
        delay(500);
        digitalWrite(A8,LOW);
    }
    if(led == '9'){
        digitalWrite(A9,HIGH);
        delay(500);
        digitalWrite(A9,LOW);
    }
    if(led == '4'){
        digitalWrite(A4,HIGH);
        delay(500);
        digitalWrite(A4,LOW);
    }
    if(led == '5'){
        digitalWrite(A5,HIGH);
        delay(500);
        digitalWrite(A5,LOW);
    }
    if(led == '6'){
        digitalWrite(A6,HIGH);
        delay(500);
        digitalWrite(A6,LOW);
    }
    if(led == '1'){
        digitalWrite(A1,HIGH);
        delay(500);
        digitalWrite(A1,LOW);
    }
    if(led == '2'){
        digitalWrite(A2,HIGH);
        delay(500);
        digitalWrite(A2,LOW);
    }
    if(led == '3'){
        digitalWrite(A3,HIGH);
        delay(500);
        digitalWrite(A3,LOW);
    }
  }

  
  
  
}
