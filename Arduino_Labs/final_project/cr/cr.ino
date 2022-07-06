#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>

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
#define SS_PIN_1             (49) //ss_safe
#define SS_PIN_2             (48) //jacob
//-----------------------------------------------------
#define LM35_PIN              A0
#define EMERGENCY_BUTTON      34
#define YELLOW_LED            40
#define RED_LED               41
#define BUZZER                42
//-----------------------------------------------------
#define HIGH_LIMIT            100
#define LOW_LIMIT             50
#define CRITICAL_TEMPERATURE  120
//-----------------------------------------------------
#define ALARM                 35
//-----------------------------------------------------
#define NORMAL_COMPARATIVELY  0.15
//-----------------------------------------------------
// lenght of the robs = 10m
// ray of the servos  = 5m
// perimeter of the servos = 5*5*pi
// renge of the tempreture = 0-159 oC
// celerity of reaction = 30
// tempreture = 50-100
// 30oC ---> 1m
//-----------------------------------------------------
// 'n' : normal
// 'l' : lower
// 'h' : higher
// 'c' : critical
char robs_status = 'n';

int ss_num = 2;
int ss_pins[] = {SS_PIN_1,SS_PIN_2};
bool emergency_mood = false;

const int NUM_SERVO = 4;
int servos_pin[] = {36,37,38,39};
Servo servos_array[NUM_SERVO];



void setup() {
  // slaves pins
  pinMode(SS_PIN_1,OUTPUT);
  pinMode(SS_PIN_2,OUTPUT);
  pinMode(MOSI,OUTPUT); 
  pinMode(MISO,INPUT);
  pinMode(SCK,OUTPUT);
  //master config
  digitalWrite(SS_PIN_1, HIGH); // disable Slave Select
  digitalWrite(SS_PIN_2, HIGH); // disable Slave Select

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8
  //buttons and leds
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LM35_PIN,INPUT);
  pinMode(EMERGENCY_BUTTON,INPUT);
  // initialize leds
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  // initialize servos
  for (int i=0; i<NUM_SERVO; i++){
      servos_array[i].attach(servos_pin[i]);
      servos_array[i].write(90.0);
  }
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REACTOR STARTED");
  delay(500);
  lcd.clear();
}



void loop() {
    check_temperature(robs_status);
    if(robs_status=='c'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("<-- WARRNING -->");
        lcd.setCursor(0,1);
        lcd.print("STOP ALL PROCESS");
        change_robs_status(180,1);
        alarm(true);
        while(robs_status=='c'){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("EMERGENCY MOOD__");
              lcd.setCursor(0,1);
              lcd.print("STATUS: CRITICAL");
              delay(500);
              check_temperature(robs_status);
        }
        digitalWrite(RED_LED,LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">-- WARRNING --<");
        lcd.setCursor(0,1);
        lcd.print("RESUME PROCESSES");
        alarm(false);
    }else if(robs_status=='l'){
        float temper = ((500.0)*analogRead(LM35_PIN))/1023; 
        change_robs_status(-temper,10);
    }else if(robs_status=='h'){
        float temper = ((500.0)*analogRead(LM35_PIN))/1023; 
        change_robs_status(temper,10);
    }else if(robs_status=='n'){}
    
}// end loop

//========================================================================================
void change_robs_status(int temp, int del){
//   float degree = map(temp,-150,150,0,180);
   float degree = temp;
   for (int i=0; i<NUM_SERVO; i++){
       servos_array[i].write(degree);
       delay(del);
   }
 }
//----------------------------------------------
void check_temperature(char &r_status){
    lcd.clear();
    float am_t = ((500.0)*analogRead(LM35_PIN))/1023; 
    lcd.setCursor(0, 0);
    lcd.print("TEMP:");
    lcd.print(am_t);
    lcd.print((char)223);
    lcd.print("C");
    if(am_t>=CRITICAL_TEMPERATURE){
        digitalWrite(RED_LED,HIGH);
        digitalWrite(YELLOW_LED,HIGH);
        lcd.setCursor(0,1);
        lcd.print("STATUS: CRITICAL");
        digitalWrite(BUZZER,HIGH);
        delay(500);
        digitalWrite(BUZZER,LOW);
        r_status = 'c';
    }
    else if(am_t>=HIGH_LIMIT){
        digitalWrite(YELLOW_LED,HIGH);
        lcd.setCursor(0,1);
        lcd.print("STATUS: HIGHER");
        digitalWrite(BUZZER,HIGH);
        delay(500);
        digitalWrite(BUZZER,LOW);
        r_status = 'h';
    }
    else if(am_t<=LOW_LIMIT){
        digitalWrite(YELLOW_LED,HIGH);
        lcd.setCursor(0,1);
        lcd.print("STATUS: LOWER");
        digitalWrite(BUZZER,HIGH);
        delay(500);
        digitalWrite(BUZZER,LOW);
        r_status = 'l';
    }else{
        if (digitalRead(YELLOW_LED)== HIGH){
            digitalWrite(YELLOW_LED,LOW);
        }
        lcd.setCursor(0,1);
        lcd.print("STATUS: NORMAL");
        r_status = 'n';
        delay(500);
      
    }
}

//----------------------------------------------
void alarm(bool emergency_mood){
  if(emergency_mood==true){
     for(int ss=0; ss<ss_num; ss++){
          send_recive_message(ss_pins[ss],"c\r");
     }
  }else{  
     for(int ss=0; ss<ss_num; ss++){
          send_recive_message(ss_pins[ss],"n\r");
     }  
  }
}
//----------------------------------------------
void send_recive_message(int ss,String message){
    byte recive;
    digitalWrite(ss, LOW); // enable Slave Select
    for (int i=0; i<message.length(); i++){
          recive = SPI.transfer(char(message[i]));
    }
    digitalWrite(ss, HIGH); // disable Slave Select
    delay(200);
}
