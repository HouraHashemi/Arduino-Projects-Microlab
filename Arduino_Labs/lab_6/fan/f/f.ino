#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

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
#define POWER_BUTTON      30
#define VELOCITY_BUTTON   31 
#define SWEEP_BUTTON      32 
#define AUTO_BUTTON       33

#define SWEEP_SERVO_PIN   49
#define RELAY_SWEEP       50

#define MOTOR_SERVO_PIN   52
#define RELAY_MOTOR       53

#define BUZZER_PIN        23
//----------------------------------------------------
Servo srv_sweep;
Servo srv_motor;
//----------------------------------------------------

int speed_defult[3] = {100,200,300};

int period_defult;

void setup() {
  
  pinMode(POWER_BUTTON,INPUT);
  pinMode(VELOCITY_BUTTON,INPUT);
  pinMode(SWEEP_BUTTON,INPUT);
  pinMode(AUTO_BUTTON,INPUT);

  pinMode(45,OUTPUT);//led slow
  pinMode(46,OUTPUT);//led med
  pinMode(47,OUTPUT);//led fast
  
  pinMode(RELAY_SWEEP,OUTPUT);
  srv_sweep.attach(SWEEP_SERVO_PIN);

  pinMode(RELAY_MOTOR ,OUTPUT);
  srv_motor.attach(MOTOR_SERVO_PIN);
  
  digitalWrite(POWER_BUTTON,LOW);
  digitalWrite(VELOCITY_BUTTON,LOW);
  digitalWrite(SWEEP_BUTTON,LOW);
  digitalWrite(AUTO_BUTTON,LOW);
  digitalWrite(RELAY_MOTOR,LOW);
  digitalWrite(RELAY_SWEEP,LOW);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|FAN __________|");
  lcd.setCursor(0, 1);
  lcd.print("|PRODUCT _2020_|");
  delay(1000);
  lcd.clear();
  // put your setup code here, to run once:
}



void loop() {
  if(digitalRead(POWER_BUTTON)==HIGH){
    power();
    
    while(true){
        
        if(digitalRead(VELOCITY_BUTTON)==HIGH){
            change_velocity(speed_defult);
        }
        if(digitalRead(SWEEP_BUTTON)==HIGH){
            sweep(5);
        }
        if(digitalRead(AUTO_BUTTON)==HIGH){
            auto_mod();
        }
    }
  }
  
}// end loop
//========================================================================================
void power(){
  digitalWrite(RELAY_MOTOR,HIGH);
//  for(int pos = srv_motor.read(); pos >= 0; pos -= 1){
//      srv_door.write(pos);            
//      delay(30);
//  }
  
}
//----------------------------------------------
void change_velocity(int *sp){
   int ss = srv_motor.read();
}
//----------------------------------------------
void sweep(int per){
  digitalWrite(RELAY_SWEEP,HIGH);
}
//----------------------------------------------
void auto_mod(){}
//----------------------------------------------
void switch_value(){//low or high
//  digitalWrite()
  
}
//----------------------------------------------
String get_period(){
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

void get_speed(int *arr) {
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
         if((hour>=96)||(minute>=60)||(second>=60)){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("__ERROR_______");
            lcd.setCursor(0, 1);
            lcd.print("INVALIED TIMER");
            delay(1500);
            lcd.clear();
            digit = 0;
            tim="";
          }
          else{
            arr[0] = hour;
            arr[1] = minute;
            arr[2] = second;
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

void fan_status(){
//  if((srv_door.read()==180)&&(srv_lock.read()==180)&&(sensor_temperature()==true)){
//      lcd.setCursor(0,1);
//      lcd.print("STATUS: OPEN");
//  }
//  else if((srv_door.read()==0)&&(srv_lock.read()==0)&&(sensor_temperature()==true)){
//      lcd.setCursor(0,1);
//      lcd.print("STATUS: CLOSE");
//  }else{
//      lcd.clear();
//      lcd.print("____INSECURE____");
//      alarm();
//  }
}
//----------------------------------------------------------------------------------------
