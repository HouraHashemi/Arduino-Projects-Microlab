#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
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
byte rowPins[ROWS] = {A9, A10, A11, A8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A13, A14, A15}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//----------------------------------------------------
#define DS18S20  A0
//----------------------------------------------------
#define DOOR 50
#define LOCK 51
//----------------------------------------------------
#define DEVICE_ADDRESS 0b10100000
#define PASS_ADDRESS  400
#define TIMER_ADDRESS 300
//----------------------------------------------------
Servo srv_lock;
Servo srv_door;

String PASSWORD = "00000";
int TIMER[3] = {0,0,5};

void setup() {
  pinMode(DS18S20,INPUT);
  pinMode(22,INPUT);//alarm
  pinMode(53, OUTPUT);//switch
  
  srv_door.attach(DOOR);//door
  srv_door.write(0);
  srv_lock.attach(LOCK);//lock
  srv_lock.write(0);

//  eeprom_read(PASS_ADDRESS,PASSWORD,5);
//  eeprom_read(TIMER_ADDRESS,TIMER,3);
//  for(int i=0; i<3; i++){
//    lcd.print(TIMER[i]);
//  }
//  delay(3000);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|SAFE _________|");
  lcd.setCursor(0, 1);
  lcd.print("|PRODUCT _2020_|");
  delay(1000);
  lcd.clear();
  // put your setup code here, to run once:
}



void loop() {
  lcd.setCursor(0,0);
  lcd.print("OPEN SAFE: [#]");
  lcd.setCursor(0,1);
  lcd.print("SETTING  : [*]");
  char key;  
  key = keypad.getKey();
  if(key){
    if(key == '#'){ //requested to unlock safe
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ENTER PASSWORD:");    
        String pass = get_password();
        lcd.clear();
        if(pass == PASSWORD){
            lcd.print("CORRECT PASSWORD");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SAFA IS OPENING");
            lcd.setCursor(0,1);
            lcd.print("PLEASE WAIT..");
            open_safe();
            lcd.clear();
            bool timer = run_timer(TIMER[0],TIMER[1],TIMER[2]);
            lcd.clear();
            if(timer){
              lcd.setCursor(0,0);
              lcd.print("SAFA IS CLOSING");
              lcd.setCursor(0,1);
              lcd.print("PLEASE WAIT..");
              close_safe();
              lcd.clear();
              safe_status();
              delay(1000);
              lcd.clear();
            }
        }else{
            lcd.print("WORONG PASSWORD!");
            delay(1000);
        }
        lcd.clear();
    }
    else if(key == '*'){//requested for change password
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CHANGE PASS :[1]");
        lcd.setCursor(0,1);
        lcd.print("CHANGE TIMER:[2]"); 
        while(true){   
          key = keypad.getKey();
          if(key){
            if(key == '1'){ //requested to unlock safe
                change_password(PASSWORD);
                break;
            }else if(key == '2'){//requested for change password
                 lcd.clear();
                 lcd.setCursor(0,0);
                 lcd.print("ENTER PASSWORD:");    
                 String pass = get_password();
                 lcd.clear();
                 if(pass == PASSWORD){
                    lcd.print("CORRECT PASSWORD");
                    change_timer(TIMER);
                 }else{
                    lcd.print("WORONG PASSWORD!");
                    delay(1000);
                 }
                 lcd.clear();
                 break;
             }
          }//end second if(key)
        }//end of while(true)
    }//end of if('*')
      
  }//end first if(key)


  
}// end loop
//========================================================================================
String get_password(){
  char key;
  String input;
  char counter = 0;
  //get password from user char by char
  while(true){
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
      //check if password lenght is 5, it match or not  
      if(counter==5){
              delay(500);
              return input;
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
  delay(1000);
  lcd.clear();
  return "";
}
//----------------------------------------------------------------------------------------
bool change_password(String &pass){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OLD PASSWORD:");
  String old_pass = get_password();
  if(old_pass==pass){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("NEW PASSWORD:");
      String new_pass = get_password();
      pass = new_pass;
//      eeprom_write(PASS_ADDRESS,pass,5);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PASSWORD CHANGED");
      delay(1000);
      lcd.clear();
      return true;
  }else{
      lcd.print("WRONGE  PASSWORD");
      delay(1000);
      lcd.clear();
      return false;
    }
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
//            eeprom_write(TIMER_ADDRESS,arr,3);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("NEW TIMER SETED");
            delay(1500);
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

void safe_status(){
  if((srv_door.read()==180)&&(srv_lock.read()==180)&&(sensor_temperature()==true)){
      lcd.setCursor(0,1);
      lcd.print("STATUS: OPEN");
  }
  else if((srv_door.read()==0)&&(srv_lock.read()==0)&&(sensor_temperature()==true)){
      lcd.setCursor(0,1);
      lcd.print("STATUS: CLOSE");
  }else{
      lcd.clear();
      lcd.print("____INSECURE____");
      alarm();
  }
}
//----------------------------------------------------------------------------------------
void open_safe(){
  digitalWrite(53,HIGH);
  srv_lock.write(180);
  delay(500);
  for(int pos = 0; pos <= 180; pos += 1){
      srv_door.write(pos);             
      delay(30);
   }
  //buzzer act
  digitalWrite(22,HIGH);
  delay(1000);
  digitalWrite(22,LOW);

}
//----------------------------------------------------------------------------------------
void close_safe(){
    for(int pos = srv_door.read(); pos >= 0; pos -= 1){
      srv_door.write(pos);            
      delay(30);
   }
  digitalWrite(53,LOW);
  srv_lock.write(0);
  delay(500);
  //buzzer act
  digitalWrite(22,HIGH);
  delay(1000);
  digitalWrite(22,LOW);

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
              safe_status();
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
//----------------------------------------------
void alarm(){
  while(true){            
      digitalWrite(22,HIGH);
      delay(100);
      digitalWrite(22,LOW);
      delay(100);
  }
}

//----------------------------------------------
bool sensor_temperature(){
    float sen = ((500.0)*analogRead(DS18S20))/1023; 
    if((sen>=60) || (sen<=5)){
      return false;//too high or too low
    }
    return true;//normal
}
//----------------------------------------------

void eeprom_write(uint8_t memory_address, int *data, int _size){
    lcd.clear();
    lcd.print("CONFIGURING..");
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((uint8_t)((memory_address & 0xFF00) >> 8));
    Wire.write((uint8_t)((memory_address & 0x00FF) >> 0));
    for(int i=0; i<_size; i++){
        Wire.write(data[i]);
        delay(500);
    }
    Wire.endTransmission();
}
//----------------------------------------------
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
        delay(300);
    }
    lcd.clear();
}
