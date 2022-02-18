#include <Keyboard.h>
#include <Keypad.h> 
#include <string.h> 
#include <EEPROM.h> 
#include <TimeLib.h>
#include <Time.h>
#include <stdlib.h> 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 7);
const byte ROWS = 4; // 4 строки 
const byte COLS = 3; // 4 столбца 
char keys[ROWS][COLS] = { 
{'1','2','3'}, 
{'4','5','6'}, 
{'7','8','9'}, 
{'*','0','#'} 
}; 
byte rowPins[ROWS] = {13,12, 11,10}; 
byte colPins[COLS] = {9, 6, 5}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 
int senorR_2=0,i=0; 
int sensorReading = 0; 
char pass_good[]="000", pass_bad[]="111";
int t_start,t_end;
char msg;
char call;
int resetPin = 4;

 
//Реакция на сигнал проникновения 
void alarm(){ 
int cout=0; 
int tr,x,y; 
t_start=second();
t_end=second()+10;
tr=0;
while (t_start<t_end||tr<3){  
char key = keypad.getKey(); 
if(key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'){pass_bad[tr]=key;tr++;}
if(tr==3) break;
Serial.println(tr);
if(t_end-t_start<=0) break; 
t_start=second();

} 

x=atoi(pass_good); 
y=atoi(pass_bad);
Serial.println(y);
if(x==y) {
  Serial.println("alarm"); 
  Reset();}
else {SendMessage();}
} 


///////////////////////
void Reset(){
            digitalWrite(resetPin, LOW);
            delay(3000);
            digitalWrite(resetPin, HIGH);
            Serial.println("reset");
            }

void RSMessage()
    {
     mySerial.println("AT+CMGL=\"ALL\"");  
     delay(1000);
    }

void SendMessage()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"89629329891\"\r");
  delay(1000);
  mySerial.println("ALARM");
  delay(100);
  mySerial.println((char)26);
  Serial.println("send"); 
  mySerial.println("AT+CMGDA=\"DEL SENT\"");
  delay(5000);
  Reset();
}


//функция смены пароля (для набора *** дается 5 секунд)
void change_pass(char n){ 
int cout=1; 
int i,x,y; 
t_start=second();
  t_end=second()+5;
  while(t_start<t_end||cout<=3){ 
char key = keypad.getKey(); 
if (key=='*') {cout++; Serial.println( cout);} 
t_start=second();
if(cout>=3) break;
if(t_end-t_start<=0) break;
}

if (t_end-t_start==0)Serial.println( "Time out"); 
if (cout==3){ 
Serial.println( "input old pass");

for (i=0;i<3;i++){ 
char key = keypad.waitForKey(); 
pass_bad[i]=key; 
} 
x=atoi(pass_bad); 
y=atoi(pass_good); 
if (x==y){ 
Serial.println( "input new pass"); 
for (i=0;i<3;i++){ 
char key = keypad.waitForKey(); 
pass_good[i]=key; 
int y;
  int addr=0;
  char pass_good;
  y=atoi(pass_good);
 int val2 = y ;
 addr = 1 ;
   EEPROM.write ( addr , val2 ) ;
} 
Serial.println( "pass changed"); 

} 
else {Serial.println( "pass is wrong");} 
} 
} 




void setup() 
{ 
pinMode(2, INPUT); 
pinMode(resetPin, OUTPUT);
digitalWrite(resetPin, HIGH);

delay(200);


mySerial.begin(9600);
Serial.begin(9600); 
delay(1000);
int x,y;
  int addr=0;
  char pass_good[]="000", pass_bad[]="111";
    x=atoi(pass_bad); 
  y=atoi(pass_good);
 int val1 = x ;
 int val2 = y ;
  EEPROM.write ( addr , val1 ) ;
 addr = addr + 1 ;
   EEPROM.write ( addr , val2 ) ;
delay(1000);
 // mySerial.println("AT+CMGDA=\"DEL ALL\"");  
 // delay(1000);
  //mySerial.println("AT+CMGL=\"ALL\"");
} 

void loop() 
{ 
sensorReading = digitalRead(2); 
if (sensorReading==0) alarm(); 
//Serial.println(sensorReading);    
char key = keypad.getKey(); 
if (key=='*'){ 
change_pass(key); 
} 
/*if (Serial.read()=='s')
      SendMessage();
  if (Serial.read()=='z')
      RSMessage();
 if (mySerial.available()>0)
 Serial.write(mySerial.read());
*/
}


