#include <SPI.h>
//-----------------------------------------------------------------------------
#define MESSAGE_1   "Hello, world!\r"
#define MESSAGE_2   "Hi!\r"
#define SS_PIN_1 (48)
#define SS_PIN_2 (49)
//---------------------------------------------------------------------------
void setup (void) {
   Serial.begin(9600); //set baud rate to 115200 for usart
   Serial.println ("[MASTER]\r-----------------------------------------------");
   pinMode(SS_PIN_1,OUTPUT);
   pinMode(SS_PIN_2,OUTPUT);
   pinMode(MOSI,OUTPUT); 
   pinMode(MISO,INPUT);
   pinMode(SCK,OUTPUT);
   
   digitalWrite(SS_PIN_1, HIGH); // disable Slave Select
   digitalWrite(SS_PIN_2, HIGH); // disable Slave Select
   SPI.begin();
   SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8
}

void loop (void) {
   char c;
   byte recive;
   Serial.print("Send To Slave_1: ");
   digitalWrite(SS_PIN_1, LOW); // enable Slave Select
   // send test string
   for (const char * p = MESSAGE_1 ; c = *p; p++){
      recive = SPI.transfer (c);
      Serial.print(c);
   }
   Serial.print("\r");
   digitalWrite(SS_PIN_1, HIGH); // disable Slave Select
   delay(1000);
   //------------
   Serial.print("Send To Slave_2: ");
   digitalWrite(SS_PIN_2, LOW); // enable Slave Select
   // send test string
   for (const char * p = MESSAGE_2 ; c = *p; p++){
      recive = SPI.transfer (c);
      Serial.print(c);
   }
   Serial.print("\r");
   digitalWrite(SS_PIN_2, HIGH); // disable Slave Select
   delay(1000);
}
