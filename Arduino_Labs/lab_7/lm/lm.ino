#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>

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
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {27, 25, 23, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {50, 48, 46}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//----------------------------------------------------
#define DEVICE_ADDRESS 0b1010000
#define TIMER_ADDRESS  100
#define MOOD_ADDRESS   200

//----------------------------------------------------

int MOOD[4] = {0,0,0,0};//per wash/wash up/rinse/dehumidify
int TIMER[3] = {0,0,5};//hour/min/sec

void setup() {
  Wire.begin();
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|LAUNDRY _____ |");
  lcd.setCursor(0, 1);
  lcd.print("|PRODUCT .2020 |");
  digitalWrite(7,LOW);//stop_start button
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  eeprom_read(MOOD_ADDRESS,MOOD,4);
//  for(int i=0; i<4; i++){
//        lcd.print(MOOD[i]);
//        delay(500);
//    }
  eeprom_read(TIMER_ADDRESS, TIMER,3);
//  for(int i=0; i<3; i++){
//        lcd.print(TIMER[i]);
//        delay(500);
//    }
  lcd.clear();
}



void loop() {
  lcd.setCursor(0,0);
  lcd.print("START:[#]");
  lcd.setCursor(0,1);
  lcd.print("CHANGE CONF:[*]");  
  char key = keypad.getKey();
  if(key){
    if(key == '#'){ //requested to unlock safe
          for(int i=0; i<4; i++){
            if(MOOD[i]==1){digitalWrite(7-i,HIGH);}
          }
          bool timer = run_timer(TIMER[0],TIMER[1],TIMER[2]);
          if(timer==true){alarm();}
      }
      else if(key == '*'){//requested for change password
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SET NEW MOOD >>");
        delay(1000);
        change_mood(MOOD);
        eeprom_write(MOOD_ADDRESS, MOOD,4);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("   -- Done --   ");
        lcd.setCursor(0,1);
        lcd.print(" NEW MOOD SETED ");
        delay(1000);
        lcd.clear();
        //
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SET NEW TIMER >>");
        delay(1000);
        change_timer(TIMER);
        eeprom_write(TIMER_ADDRESS, TIMER,3);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("   -- Done --   ");
        lcd.setCursor(0,1);
        lcd.print(" NEW TIMER SETED");
        delay(1000);
        lcd.clear();
      }//end of if('*')
      
  }//end if(key)


  
}// end loop
//========================================================================================
void change_mood(int *mood){
    String arr_mood[4] = {"PER WASH >","DETERGENT WASH >","RINSE >","DEHUMIDIFY >"};
    for(int i=0; i<4;i++){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(arr_mood[i]);
        lcd.setCursor(0,1);
        lcd.print("SET[#], UNSET[*]");
        while(true){
          char key = keypad.getKey();
          if(key){
            if(key == '#'){ //requested to unlock safe
              mood[i] = 1;
            }else if(key == '*'){
              mood[i] = 0;
            }
          break;
          }//end of key
       }//end of while
    }//end of for
    
}

//----------------------------------------------------------------------------------------
void change_timer(int *arr) {
  char key;
  String tim;
  char digit = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("--:--:--");
  
  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("CONFIRM BY[#]KEY");

    key = keypad.getKey();
    if (key) {
      // delete char just entered or enter new char
      if (key == '#') {
          int hour   = tim.substring(0,2).toInt();
          int minute = tim.substring(2,4).toInt();
          int second = tim.substring(4,6).toInt();
         if((hour>=96)||(minute>=60)||(second>=60)||(digit<7)){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("__ERROR_______");
            lcd.setCursor(0, 1);
            lcd.print("INVALIED TIMER");
            delay(1500);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("--:--:--");
            digit = 0;
            tim="";
          }
          else{
            arr[0] = hour;
            arr[1] = minute;
            arr[2] = second;
            lcd.clear();
            break;
          }
      }
      else if (key == '*') {
        if((digit==3)||(digit==6))
            digit = digit - 1;
        digit = digit - 1;
        lcd.setCursor(digit, 0);
        lcd.print('-');
        String temp;
        for (int i = 0; i < tim.length() - 1; i++) {
            temp = temp + tim[i];
        }
        tim = temp;
      }
      else {
        if(digit<8){
            tim = tim + key;
            if((digit==2)||(digit==5))
              digit = digit + 1;
            lcd.setCursor(digit, 0);
            lcd.print(key);
            digit = digit + 1;
        }
      }
    }// end if(key)
  }//end while
}

//----------------------------------------------------------------------------------------
bool run_timer(int hour, int minute, int seconds) {
  //check clock has inizial value
  bool flag_h = hour>0 ? true: false;
  bool flag_m = minute>0 ? true: false;
  bool flag_s = seconds>0 ? true: false;
  
  while(hour>=0){
        //check hour>00 and minute was 00
      if((flag_m==false) && (flag_h==true)){
         hour--;
         minute = 59;
      }
      flag_h = hour>0 ? true: false;
      while(minute>=0){
            //check minute>00 and seconds was 00
          if((flag_s == false)&&(flag_m == true)){
              minute--;
              seconds = 59;
          }
          flag_m = minute>0 ? true: false;
          while(seconds>=0){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(clock_form(hour,minute,seconds));
              lcd.setCursor(0,1);
              lcd.print("P:"+String(MOOD[0])+" W:"+String(MOOD[1])+" R:"+String(MOOD[2])+" D:"+String(MOOD[3]));
              delay(1000);
              seconds--;
          }
          flag_s = seconds>0 ? true: false;
            //check hours=00 and minutes=00 and second=00
          if((flag_h==false)&&(flag_m==false)&&(flag_s==false)){return true;}
          //end second loop
       }//end minute loop
    }//end hour loop 
  return false;   
}
//----------------------------------------------
String clock_form(int hour, int minute, int second) {
  String c_form;
  String temp_m;
  String temp_h;
  String temp_s;
   if (second < 10)
    temp_s = "0" + String(second, DEC);
  else
    temp_s = String(second, DEC);
  //check minute form
  if (minute < 10)
    temp_m = "0" + String(minute, DEC);
  else
    temp_m = String(minute, DEC);
  // check hour form
  if (hour == 0)
    temp_h = "00";
  else
    temp_h = "0" + String(hour, DEC); 
  // arrenge clock form like 00:00:00
  c_form = temp_h + ":" + temp_m;
  c_form = c_form + ":" + temp_s;
  return c_form;
}


//----------------------------------------------------------------------------------------
void eeprom_write(uint8_t memory_address, int *data, int _size){
    lcd.clear();
    lcd.print("CONFIGURING..");
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((uint8_t)((memory_address & 0xFF00) >> 8));
    Wire.write((uint8_t)((memory_address & 0x00FF) >> 0));
    for(int i=0; i<_size; i++){
        Wire.write(data[i]);
        delay(300);
    }
    Wire.endTransmission();
}
//----------------------------------------------------------------------------------------
void eeprom_read(uint8_t memory_address, int *data, int _size){
    lcd.clear();
    lcd.print("RECOVER DATA..");
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((uint8_t)((memory_address & 0xFF00) >> 8));
    Wire.write((uint8_t)((memory_address & 0x00FF) >> 0));
    Wire.endTransmission();
    Wire.requestFrom(DEVICE_ADDRESS, _size);
    for(int i=0; i<_size; i++){
        data[i] = Wire.read();
        delay(100);
    }
    lcd.clear();
}
//----------------------------------------------------------------------------------------
void alarm(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LUNDERY FINISHED");
    lcd.setCursor(0,1);
    lcd.print("RESTART BY[#]KEY");
    digitalWrite(7,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(4,HIGH);
    while(true){            
        char key = keypad.getKey();
        if(key){
          if(key =='#')
            break;
        }
    }
    digitalWrite(7,LOW);
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    lcd.clear();
}
