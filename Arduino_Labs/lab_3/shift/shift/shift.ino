#include <LiquidCrystal.h>

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11


LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

String my_name = "NOURA";
int line = 0;
int start = 0;

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" PROGRAMED  BY: ");
  lcd.setCursor(0,1);
  lcd.print("   NOURA .S.H   ");
  delay(3000);
  // put your setup code here, to run once:
}

void loop() {
  lcd.clear();
  lcd.setCursor(start,line);
  lcd.print(my_name);
  if (start == 10){
    start = 0;
    if(line == 1){
      line = 0;
    }else {
      line = 1;
    }
  }
  start = start + 1;
  delay(1000);
  // put your main code here, to run repeatedly:

}
