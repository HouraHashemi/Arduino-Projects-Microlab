#include "pitches.h"
#include "themes_3.h"
//-----------------------------------------
#define SOUNDER_PIN   52
#define BUTTON_PIN    50
#define POTENTIOMETER A0
//-----------------------------------------

float scale = 0;

void setup() {
  pinMode(BUTTON_PIN, OUTPUT);
  pinMode(SOUNDER_PIN, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);
  digitalWrite(BUTTON_PIN, LOW);
}

void loop() {
  
  if(digitalRead(BUTTON_PIN)==HIGH){
    for (int thisNote = 0; thisNote < (sizeof(noteDurations)/sizeof(int)); thisNote++) {
      scale = analogRead(POTENTIOMETER)/512.0;
      int noteDuration = 1000/noteDurations[thisNote];
      tone(SOUNDER_PIN, (int)(melody[thisNote]*scale), noteDuration);
      delay((int)(sizeof(noteDurations)/sizeof(noteDurations[thisNote])*1.3));
    }
    digitalWrite(BUTTON_PIN,LOW);
    delay(500);
  }

}//end of loop
