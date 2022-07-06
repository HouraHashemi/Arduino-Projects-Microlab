#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
char buff [50];
volatile byte indx;
volatile boolean process;

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
//----------------------------------------------------

const byte ROWS = 4; //fore rows
const byte COLS = 3; //three columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'7','8','9'},
  {'4','5','6'},
  {'1','2','3'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A1, A2, A3,A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A6, A5, A4}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//-----------------------------------------------------
#define EMERGENCY_MOOD 47
//-----------------------------------------------------

const String PASSWORD = "1234567";
const int NUM_SERVO = 12;

bool configuration_mod = false;
bool pass = false;

int servos_pins[] = {22,23,24,25,30,31,32,33,38,39,40,41};
int led_pins[]    = {26,27,28,29,34,35,36,37,42,43,44,45};
Servo servos_array[NUM_SERVO];

String message = "";


void setup() {
  pinMode(EMERGENCY_MOOD,OUTPUT);

  pinMode(SS,INPUT);
  pinMode(MOSI,INPUT); 
  pinMode(MISO,OUTPUT);
  pinMode(SCK,INPUT);
  digitalWrite(SS,HIGH);
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  indx = 0; // buffer empty
  process = false;
  SPI.attachInterrupt(); // turn on interrupt

  pinMode(EMERGENCY_MOOD,OUTPUT);

  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("--+ WELLCOME +--");
  lcd.setCursor(0,1);
  lcd.print(" JACOB ELECTRIC ");
  delay(1000);
  lcd.clear();
//set servos and leds position and map them to thier pins
  for (int i=0; i<NUM_SERVO; i++){
      pinMode(led_pins[i],OUTPUT);
      digitalWrite(led_pins[i], HIGH);
      servos_array[i].attach(servos_pins[i]);
      servos_array[i].write(0.0);
  }
  // put your setup code here, to run once:
}


ISR (SPI_STC_vect){ // SPI interrupt routine  
   byte c = SPDR; // read byte from SPI Data Register
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r'){ //check for the end of the word
        process = true;
      }
   }
}


void loop() {
  message = recive_message();
  if(message == "c\r"){
     emergency_mood();
  }
  char key;
  lcd.setCursor(0,0);
  lcd.print("RECHARGE --> [#]");
  lcd.setCursor(0,1);
  lcd.print("BUY FOOD --> [*]");
  
  key = keypad.getKey();
  if(key){
    if(key == '#'){ //rechagre machine
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ENTER  PASSWORD:");
        pass = check_password();
        lcd.clear();
        if(pass == true){
            lcd.print("CORRECT PASSWORD");
            recharge_machine();
        }else{
            lcd.print("WORONG PASSWORD!");
            lcd.setCursor(0,1);
            lcd.print("   TRY AGAIN    ");
        }
      delay(1000);
      pass = false;
    }
    else if(key == '*'){//buy food
        lcd.clear();
        get_food();
      }
  }


}// end loop
//========================================================================================
String recive_message(){
    String mess="";
    if (process) {
        process = false; //reset the process
        mess = buff;
        indx= 0; //reset button to zero
        return mess;
    }
}
//----------------------------------------------------------
// this function cheking password which just entered is correct or not
bool check_password(){
  char key;
  String input;
  char counter = 0;
  //get password from user char by char
  while(true){
    message = recive_message();
    if(message == "c\r"){
        emergency_mood();
        break;
    }
    key = keypad.getKey();
    if(key){
      // delete char just entered or enter new char
      if((key == '*') && (counter!=1)){
          counter = counter - 1;
          lcd.setCursor(counter,1);
          lcd.print(' ');
      }
      else{
          input = input + key;
          lcd.setCursor(counter,1);
          lcd.print(key);
          counter=counter+1;
      }
      //check if password lenght is 7, it match or not  
      if(counter==7){
          if(input == PASSWORD) 
              return true;
          else
              return false;
      }else{
          continue;
      }
      break;   
    }//if get key
  }//end while
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-ERROR OCCOURED-");
  lcd.setCursor(0,1);
  lcd.print("--RESET NEEDED--");
  return false;
}
//-------------------------------------------


void recharge_machine(){
    message = recive_message();
    if(message == "c\r"){
      emergency_mood();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOR STATE: OPEN");
    lcd.setCursor(0,1);
    lcd.print("CLOSE -----> [#]");

    for(int i=0; i<NUM_SERVO; i++){
        digitalWrite(led_pins[i],LOW);
        servos_array[i].write(180);
        delay(500);               
    }
    //for get out this state enter #
    while(keypad.waitForKey()!='#'){
        message = recive_message();
        if(message == "c\r"){
            emergency_mood();
        }
    };
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DOOR STATE:CLOSE");
    delay(1000);
    for(int i=0; i<NUM_SERVO; i++){
        digitalWrite(led_pins[i],HIGH);
    }    
}
//------------------------------------------------------

 
void get_food(){
  char key;
  String food_number;
  char cr = 1;// for moving cursor
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CHOOSE NUM & [#]");
  lcd.setCursor(0,1);
  lcd.print(":");
  // get number from user
  while(true){
    message = recive_message();
    if(message == "c\r"){
        emergency_mood();
        break;
    }
    key = keypad.getKey();
    if(key){
      // delete char just entered or enter new char
      if(key == '#'){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("- PLEASE  WAIT -");
          delay(300);
          // this block check if food exist or not
          int ch = NUM_SERVO + 1;
            for(int i=0; i<NUM_SERVO; i++){
              if(food_number.toInt() == servos_pins[i]){
                ch = i;
                for(int j=0; j<3; j++){
                  digitalWrite(led_pins[i],LOW);
                  delay(300);
                  digitalWrite(led_pins[i],HIGH);
                  delay(300);
                }
                break;
              }
            }
          //checking food
          if( ch != NUM_SERVO+1 ){   // && (servos_array[food_number.toInt()].read()>=0)){
              int current_state = servos_array[ch].read();
              servos_array[ch].write(-(current_state - 90));
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("DONE.   ^______^");
              lcd.setCursor(0,1);
              lcd.print("ENJOY YOUR FOOD!");
              delay(3000);
              food_number = "";
              cr = 1;
              break;
          }else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("-ERROR OCCOURED-");
              lcd.setCursor(0,1);
              lcd.print("-- TRY  AGAIN --");
              delay(3000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("CHOOSE NUM & [#]");
              lcd.setCursor(0,1);
              lcd.print(":");
              food_number = "";
              cr = 1;
          }
      }
      else if((key == '*') && (cr!=1)){
            cr = cr - 1; 
            lcd.setCursor(cr,1);
            lcd.print(' '); 
            String temp;
            for (int i=0; i<food_number.length()-1; i++){
                temp = temp + food_number[i];
            }
            food_number = temp;
      }
      else{
          food_number = food_number + key;
          lcd.setCursor(cr,1);
          lcd.print(key);
          cr=cr+1;
      }
  
    }// end if(key)
  }//end while
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-ERROR OCCOURED-");
  lcd.setCursor(0,1);
  lcd.print("--RESET NEEDED--");
  return false;
}
//------------------------------------------------------
void emergency_mood(){
    digitalWrite(EMERGENCY_MOOD,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("<-- WARRNING -->");
    lcd.setCursor(0,1);
    lcd.print("STATUS: CRITICAL");
    String mess = "";
    while(mess != "n\r"){
         mess = recive_message();
    }
    digitalWrite(EMERGENCY_MOOD,LOW);
}
 
  
