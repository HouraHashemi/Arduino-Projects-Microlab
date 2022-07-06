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
byte rowPins[ROWS] = {24, 23, 22, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 30, 29}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//-----------------------------------------------------
#define LM35_PIN  A0
//----------------------------------------------------

Servo servo;
int timer[2]={0,0};
int temperature_required = 0;
int ambient_temperature;

void setup() {
  pinMode(LM35_PIN,INPUT);
  pinMode(40,INPUT);//emergency mode
  pinMode(46, OUTPUT);//led
  pinMode(47, OUTPUT);//buzzer
  digitalWrite(46, HIGH);
  servo.attach(45);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("* SOLAR  STOVE *");
  lcd.setCursor(0, 1);
  lcd.print("| PRODUCT 2020 |");
  delay(2000);
  lcd.clear();
  // put your setup code here, to run once:
}



void loop() {
  fill_stove();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("READY,START :[#]");
  lcd.setCursor(0, 1);
  lcd.print("     ,CANCLE:[*]");
  char key;

  while(true){
    key = keypad.getKey();
    if (key) {
      if (key == '#') { //get instruction about cooking
        get_temperature(temperature_required);
        get_time(timer);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(temperature_required);
        lcd.print((char)223);
        lcd.print("C | ");
        lcd.print(clock_form(timer[0],timer[1],0));
        lcd.setCursor(0, 1);
        lcd.print("CONFIRM BY[#]KEY"); 
        while (keypad.waitForKey() != '#');
        bool check_temper = reach_desire_temperature(temperature_required);
        if(check_temper){
          //bool check_timer = run_timer(1,0,5);
          bool check_timer = run_timer(timer[0],timer[1],0);
          if (check_timer){alarm();}
        //end of cooking process
        }else{
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("_UNSUCCESSFULU_");
        }
        break;
      }
      else if (key == '*') {
        lcd.clear();
        delay(1000);
        break;
      }
      else{
        continue;
      }
    }//end of if(key)
  }//end of while(true)


}// end loop
//========================================================================================

void get_temperature(int &degrees_c) {
  char key;
  String temper;
  char digit = 12;
  lcd.clear();

  while (true) {
    lcd.setCursor(0, 0);
    lcd.print("TEMPERATURE:");
    lcd.setCursor(0, 1);
    lcd.print("CONFIRM BY[#]KEY");

    key = keypad.getKey();
    if (key) {
      // delete char just entered or enter new char
      if (key == '#') {
        //checking food
        if ( (temper.toInt() <= 300) && (temper.length() > 0)) {
          degrees_c = temper.toInt();
          break;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("-ERROR OCCOURED-");
          lcd.setCursor(0, 1);
          lcd.print("-- TRY  AGAIN --");
          delay(1000);
          lcd.clear();
          temper = "";
          digit = 12;
        }
      }
      else if ((key == '*') && (digit > 12)) {
        digit = digit - 1;
        lcd.setCursor(digit, 0);
        lcd.print(' ');
        String temp;
        for (int i = 0; i < temper.length() - 1; i++) {
          temp = temp + temper[i];
        }
        temper = temp;
      }
      else {
        temper = temper + key;
        lcd.setCursor(digit, 0);
        lcd.print(key);
        digit = digit + 1;
      }
    }// end if(key)
  }//end while
}

//----------------------------------------------
void get_time(int *ar) {
  char key;
  String tim;
  char digit = 10;
  
  lcd.clear();
  while (true) {
    lcd.setCursor(0, 0);
    lcd.print("TIME(MIN):");
    lcd.setCursor(0, 1);
    lcd.print("CONFIRM BY[#]KEY");

    key = keypad.getKey();
    if (key) {
      // delete char just entered or enter new char
      if (key == '#') {
        //checking food
        if ( (tim.toInt() <= 300) && (tim.length() > 0)) {
            ar[0] = tim.toInt()/ 60;
            ar[1] = tim.toInt()% 60;
            break;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("-ERROR OCCOURED-");
          lcd.setCursor(0, 1);
          lcd.print("-- TRY  AGAIN --");
          delay(2000);
          lcd.clear();
          tim = "";
          digit = 10;
        }
      }
      else if ((key == '*') || (digit < 10)) {
        digit = digit - 1;
        lcd.setCursor(digit, 0);
        lcd.print(' ');
        String temp;
        for (int i = 0; i < tim.length() - 1; i++) {
            temp = temp + tim[i];
        }
        tim = temp;
      }
      else {
        tim = tim + key;
        lcd.setCursor(digit, 0);
        lcd.print(key);
        digit = digit + 1;
      }
    }// end if(key)
  }//end while
}

//----------------------------------------------
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
              emergency_button();
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(clock_form(hour,minute,seconds));
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MEAL IS READY ^^");
  digitalWrite(47,HIGH);
  delay(2000);
  digitalWrite(47,LOW);
}

//----------------------------------------------
bool reach_desire_temperature(int dt){
    float am_t = ((500.0)*analogRead(LM35_PIN))/1023; 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMP:");
    lcd.setCursor(0, 1);
    lcd.print("----------------");
    int counter = 0;
    while(am_t <= dt){
          emergency_button();
          if (counter>7200){
              return false;
          }
          am_t = ((500.0)*analogRead(LM35_PIN))/1023;
          lcd.setCursor(8, 0);
          lcd.print(am_t);
          lcd.print((char)223);
          lcd.print("C");
          delay(500);
          counter = counter+1;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("STOVE HAS REACHED");
    lcd.setCursor(0, 1);
    lcd.print("TEMPERATURE:");
    lcd.print(am_t);
    digitalWrite(47,HIGH);//buzzer
    delay(500);
    digitalWrite(47,LOW);
    delay(500);
    return true;
}
//----------------------------------------------
void fill_stove() {
    //open the glassdoor
  lcd.setCursor(0, 1);
  lcd.print("OPEN BY [#] KEY");
  while (keypad.waitForKey() != '#');
    //change glassdoor position 
  lcd.clear();
  lcd.print("PLEASE WAIT...");
  servo.write(180);
  delay(1000);
  digitalWrite(46, LOW);
    //close the glassdoor
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("CLOSE BY [#] KEY");
  while (keypad.waitForKey() != '#');
    //change glassdoor position 
  lcd.clear();
  lcd.print("PLEASE WAIT...");
  servo.write(0);
  delay(1000);
  digitalWrite(46, HIGH);
}
//----------------------------------------------
void emergency_button(){
   if(digitalRead(40)==HIGH){
      digitalRead(40)==LOW;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!EMERGENCY MODE!");
      lcd.setCursor(0, 1);
      lcd.print("!RESET REQUIRED!");
      servo.write(180);
      delay(1000);
      while(true){
        digitalWrite(47,LOW);
        digitalWrite(46,LOW);
        delay(500);
        digitalWrite(47,HIGH);
        digitalWrite(46,HIGH);
        delay(500);
      }
   }
}
