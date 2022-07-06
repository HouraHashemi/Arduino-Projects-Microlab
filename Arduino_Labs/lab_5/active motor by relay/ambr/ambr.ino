/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#include <LiquidCrystal.h>


#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11


LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(52, OUTPUT);

  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("----- START -----");
  delay(3000);
}

// the loop function runs over and over again forever
void loop() {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RELAY:   ACTIVE ");
  lcd.setCursor(0,1);
  lcd.print("   SERVO:  ON   ");
  digitalWrite(52, HIGH);
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RELAY:  DEACTIVE");
  lcd.setCursor(0,1);
  lcd.print("   SERVO: OFF   ");
  digitalWrite(52, LOW);
  delay(3000);
}
