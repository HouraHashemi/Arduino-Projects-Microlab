#include <SPI.h>
char buff [50];
volatile byte indx;
volatile boolean process;

void setup (void) {
   Serial.begin(9600);
   Serial.println ("[SLAVE]\r-----------------------------------------------");
   
   pinMode(SS,INPUT);
   pinMode(MOSI,INPUT); 
   pinMode(MISO,OUTPUT);
   pinMode(SCK,INPUT);
   
   digitalWrite(SS,HIGH);
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
}

ISR (SPI_STC_vect){ // SPI interrupt routine  
   byte c = SPDR; // read byte from SPI Data Register
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r') //check for the end of the word
      process = true;
   }
}

void loop (void) {
   if (process) {
      process = false; //reset the process
      Serial.println (buff); //print the array on serial monitor
      indx= 0; //reset button to zero
   }
}
