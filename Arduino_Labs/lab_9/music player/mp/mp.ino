#include "pitches.h"
#include "themes_the_imperial_march.h"
//-----------------------------------------
#define SOUNDER_PIN 52
#define BUTTON_PIN  50
//-----------------------------------------

void setup() {
  pinMode(BUTTON_PIN, OUTPUT);
  pinMode(SOUNDER_PIN, OUTPUT);
  digitalWrite(BUTTON_PIN, LOW);
}

void loop() {
  if(digitalRead(BUTTON_PIN)==HIGH){
    for (int thisNote = 0; thisNote<(sizeof(noteDurations)/sizeof(int)); thisNote++) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(SOUNDER_PIN, melody[thisNote], noteDuration);
      delay(sizeof(noteDurations)/sizeof(noteDurations[thisNote]));
    }
    digitalWrite(BUTTON_PIN,LOW);
    delay(500);
  }

}//end of loop
