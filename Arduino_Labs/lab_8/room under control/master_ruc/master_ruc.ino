#include <SPI.h>
//-----------------------------------------------------------------------------
#define SS_PIN_1 (48)
#define SS_PIN_2 (49)
//---------------------------------------------------------------------------
#define THERMOMETER (A0)
#define PHOTOCELL   (A1)
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
  
   float photocell = ((500.0)*analogRead(PHOTOCELL))/1023; 
   float thermometer = ((500.0)*analogRead(THERMOMETER))/1023;
   String ph = String(photocell)+"\r";
   String th = String(thermometer)+"\r";
   
   char c;
   byte recive;
   Serial.print("Send To Slave_1 [Photocell(mm)]: ");
   delay(500);
   digitalWrite(SS_PIN_1, LOW); // enable Slave Select
   for (int i=0; i<ph.length(); i++){
      recive = SPI.transfer (char(ph[i]));
      Serial.print(ph[i]);
   }
//   for (char * p = ph[0]; c = *p; p++){
//      recive = SPI.transfer (c);
//      Serial.print(c);
//   }
   Serial.print("\r");
   digitalWrite(SS_PIN_1, HIGH); // disable Slave Select
   //---------------------------
   Serial.print("Send To Slave_2 [Thermometer(.C)]: ");
   delay(500);
   digitalWrite(SS_PIN_2, LOW); // enable Slave Select
   for (int i=0; i<th.length(); i++){
      recive = SPI.transfer (char(th[i]));
      Serial.print(th[i]);
   }
//   for (char * p = th[0]; c = *p; p++){
//      recive = SPI.transfer (c);
//      Serial.print(c);
//   }
   Serial.print("\r");
   digitalWrite(SS_PIN_2, HIGH); // disable Slave Select
   delay(500);
}
