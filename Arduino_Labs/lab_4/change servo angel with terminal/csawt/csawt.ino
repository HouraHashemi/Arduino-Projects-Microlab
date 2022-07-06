/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

String angle = "0";

void setup() {
  Serial.begin(9600);
  Serial.println("ENTER YOUR ANGLE: (FOR CONFIRM ENTER \"e\")");
  //pinMode(A13, OUTPUT);
  myservo.attach(13);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
}

void loop() {
    if (Serial.available() > 0){  
           
        char a = Serial.read();
        
        if(a == 'e'){ // exit
            Serial.println();
            Serial.println("SERVO IS RUNNING.");
            // Serial.println(angle.toInt());
            // analogWrite(A13,angle.toInt());
            //int val = analogWrite(A13 ,angle.toInt());
            myservo.write(180 - angle.toInt());
            delay(1000);
            Serial.println("------------------------");
            angle = "0";
            Serial.println("ENTER YOUR ANGLE: ");
        }
         else{
            Serial.print(a);
            angle = angle + a;   
        }
    }             
}
