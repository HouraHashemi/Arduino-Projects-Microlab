char ch;
void setup() {
Serial.begin(9600);
pinMode(A0, OUTPUT);  
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);
pinMode(A6, OUTPUT);
pinMode(A7, OUTPUT);
pinMode(A8, OUTPUT);
Serial.println("Enter a num 1-9:");
}

void loop() {
if (Serial.available() > 0)
{       
 ch = Serial.read();
 Serial.println(ch);
 switch (ch) {
  case '1':
    digitalWrite(A0, HIGH);
    break;
  case '2':
    digitalWrite(A1, HIGH);
    break;
  case '3':
    digitalWrite(A2, HIGH);
    break;
  case '4':
    digitalWrite(A3, HIGH);
    break;
  case '5':
    digitalWrite(A7, HIGH);
    break;
  case '6':
    digitalWrite(A6, HIGH);
    break;
  case '7':
    digitalWrite(A5, HIGH);
    break;
  case '8':
    digitalWrite(A4, HIGH);
    break;
  case '9':
    digitalWrite(A8, HIGH);
    break;
  default:
    
    break;
}
}
}
