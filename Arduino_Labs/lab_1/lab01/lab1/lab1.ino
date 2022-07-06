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

// the setup function runs once when you press reset or power the board
void setup() {
pinMode(0, OUTPUT);  
pinMode(1, OUTPUT);
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(10, INPUT);
pinMode(11, INPUT);
pinMode(12, INPUT);
}


void loop() {

 if( digitalRead(10)== LOW)
{
  digitalWrite(0, HIGH);
  delay(1000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(6, HIGH);
  delay(1000);
}
 if( digitalRead(11)== LOW)
{
  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(3, HIGH);
  delay(1000);
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(1, HIGH);
  delay(1000);
  digitalWrite(0, HIGH);
  delay(1000);
}
 if( digitalRead(12)== LOW)
{
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
} 
}
