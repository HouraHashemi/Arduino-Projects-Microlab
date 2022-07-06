#include <SPI.h>
//-----------------------------------------------------------------------------
#define MESSAGE   "Hello, world!\r"
#define SS_PIN (25)
//---------------------------------------------------------------------------
void setup (void) {
   Serial.begin(9600); //set baud rate to 115200 for usart
   Serial.println ("[MASTER]\r-----------------------------------------------");
   pinMode(SS_PIN,OUTPUT);
   pinMode(MOSI,OUTPUT); 
   pinMode(MISO,INPUT);
   pinMode(SCK,OUTPUT);
   
   digitalWrite(SS_PIN, HIGH); // disable Slave Select
   SPI.begin();
   SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8
}

void loop (void) {
   char c;
   byte recive;
   digitalWrite(SS_PIN, LOW); // enable Slave Select
   delay(100);
   // send test string
   for (const char * p = MESSAGE ; c = *p; p++){
      recive = SPI.transfer (c);
      Serial.print(c);
   }
   Serial.print("\r");
   digitalWrite(SS_PIN, HIGH); // disable Slave Select
   delay(1000);
}
//====================================================================
//void send_data(uint8_t select, String message){
//    char c;
//    byte recive;
//    digitalWrite(select, LOW); // enable Slave Select
//    for (const char * p = message ; c = *p; p++){
//       recive = SPI.transfer (c);
//       Serial.print(c);
//    }
//    Serial.print("\r");
//    digitalWrite(select, HIGH); // disable Slave Select
//    delay(1000);
//}
