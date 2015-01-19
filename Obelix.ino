//Librerias
#include <SoftwareSerial.h>
#include <Bridge.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>
#include <Wire.h>

#include <EEPROM.h>

#include <DS1307RTC.h>
#include <serLCD.h>
#include <Time.h>

// Asignaciones de pines

//#define RX 0
//#define TX 1
//#define SDA 2
//#define SCL 3
#define mdir 4
#define mpow 5
#define butt1 6
//#define xx 7
#define lbutt1 8
#define lred 9
#define lgreen 10
#define lblue 11
//#define TXLCD 12
#define iled 13
#define butt2 A0
#define lbutt2 A1
#define bell A2
//#define xx A3
//#define xx A4
//#define xx A5



//Asignaciones de acciones
#define give HIGH
#define take LOW

#define ring HIGH
#define noring LOW

#define serving 15
#define highpower 150
#define lowpower 80

//Declaraciones

 // RX, TX del LCD
int dispCurMinute = 0;
int dispNextMinute = 0;
long dispNextFeedTime = 0;
int currentSecond = 0;
serLCD lcd(12);
bool wishToRing = false;

long nextFeedTime = 0;

YunServer server;


////////////////////////
//                    //
//      SETUP()       //
//                    //
////////////////////////

void setup() {
  
  
pinMode(mdir, OUTPUT);
pinMode(mpow, OUTPUT);
pinMode(butt1, INPUT_PULLUP);
pinMode(lbutt1, OUTPUT);
pinMode(lred, OUTPUT);
pinMode(lblue, OUTPUT);
pinMode(lgreen, OUTPUT);
pinMode(iled, OUTPUT);
pinMode(butt2, INPUT_PULLUP);
pinMode(lbutt2, OUTPUT);
pinMode(bell, OUTPUT);
  
digitalWrite(mdir, give);
analogWrite(mpow, 0);
digitalWrite(lbutt1, LOW);
digitalWrite(lred, LOW);
digitalWrite(lblue, LOW);
digitalWrite(lgreen, LOW);
digitalWrite(iled, LOW);
digitalWrite(lbutt2, LOW);
  
delay(800);
  
lcd.clear();
lcd.print("   Vers. 0.10   ");
lcd.selectLine(2);
lcd.print("A. Sastre - 2015");

delay(2000);

digitalWrite(lblue, HIGH);

Bridge.begin();
server.begin();


lcd.clear();

digitalWrite(lred, HIGH);

delay(2000);

setSyncProvider(RTC.get);

digitalWrite(lred, LOW);

getNextFeedTime();

  
}





////////////////////////
//                    //
//       LOOP()       //
//                    //
////////////////////////




void loop() {
  
  displayHour();
  displayNext();
  displayCountDown();
  
  YunClient client = server.accept();  // Get clients coming from server
  if (client)                          // Is there a new client?
  {
    client.print("<html><head><title>Obelix</title></head><body>");
    client.print("<h1>Obelix</h1><br>");
    client.print(nextFeedTime);
    client.print("<br>");
    
    client.print(EEPROM.read(0));
    
    client.print("<br>");
    
    for(int i=1 ; i<= int(EEPROM.read(0)) ; i++){

      client.print(EEPROMToDate(i));
      client.print("<br>");
       
     }
    client.print("</body></html>");
    client.stop();                     // Close connection and free resources.
  }
  
  if (isTheTime()){
    
    giveFood(1);
    ringBell(3);
    getNextFeedTime();
    
  } else if (digitalRead(butt1)==LOW && digitalRead(butt2)==HIGH){
    
    digitalWrite(mdir, give);
    digitalWrite(lbutt1, HIGH);
    analogWrite(mpow, highpower);
    
  }else if(digitalRead(butt1)==HIGH && digitalRead(butt2)==LOW){
    
    digitalWrite(lbutt2, HIGH);
    wishToRing = true;
    delay(10);
    
  } else if(wishToRing && digitalRead(butt2)==HIGH){
    
    ringBell(1);
    wishToRing = false;
 
  }else{
    
   digitalWrite(mdir, give);
   analogWrite(mpow, 0);
   digitalWrite(lbutt1, LOW);
   digitalWrite(lred, LOW);
   digitalWrite(lblue, LOW);
   analogWrite(lgreen, 10);
   digitalWrite(iled, LOW);
   digitalWrite(lbutt1, LOW);
   digitalWrite(lbutt2, LOW);
   
 
  }



}




/////////////////////////
//                     //
//      FUNCIONES      //
//                     //
/////////////////////////



// Alimentar //////////////////////////////////////////////////////////////////////
void giveFood(int servings){
 
digitalWrite(lbutt2, HIGH);  

digitalWrite(lred, LOW);
digitalWrite(lblue, HIGH);
digitalWrite(lgreen, LOW);

delay(1000);

digitalWrite(mdir, give);
analogWrite(mpow, highpower);

 for (int i = 0 ; i < servings ; i++) {
   for(int j = 0 ; j < serving ; j++ ) {
      
      delay(100);
      
   }
 }

digitalWrite(mdir, take);
analogWrite(mpow, lowpower);

delay(1500);

analogWrite(mpow, 0);
  
}




void displayHour(){

  if(dispCurMinute != minute()){
    
    lcd.setCursor(1,1);
    
    if(hour() < 10)
    lcd.print("0");
    lcd.print(hour(), DEC);
    
    lcd.print(":");
    
    if(minute() < 10)
    lcd.print("0");
    lcd.print(minute(), DEC);
    
    //lcd.print(":");
    
    //if(second() < 10)
    //lcd.print("0");
    //lcd.print(second(), DEC);
    
    //lcd.setCursor(2,1);
    //lcd.print(now());
    
    dispCurMinute = minute();
    
  } 
  
}


void displayNext(){
  
 if(dispNextFeedTime != nextFeedTime){
   
    lcd.setCursor(1,12);
    
    if(hour(nextFeedTime) < 10)
    lcd.print("0");
    lcd.print(hour(nextFeedTime), DEC);
    
    lcd.print(":");
    
    if(minute(nextFeedTime) < 10)
    lcd.print("0");
    lcd.print(minute(nextFeedTime), DEC);
    
    //lcd.print(":");
    
    //if(second() < 10)
    //lcd.print("0");
    //lcd.print(second(), DEC);
    
    //lcd.setCursor(2,1);
    //lcd.print(now());
    
    dispNextFeedTime = nextFeedTime;
    
  } 
  
}



void displayCountDown(){
  
 if(currentSecond != second()){
    
   long left = nextFeedTime - now();
   
   if(left >= 0){
   
    lcd.setCursor(2,9);
    
    
    
    int hours, minutes, seconds;
    
    seconds = left % 60;
    minutes = left / 60 % 60;
    hours = left / 60 / 60 % 60;
    
    if(hours < 10)
    lcd.print("0");
    lcd.print(hours);
    lcd.print(":");
    if(minutes < 10)
    lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if(seconds < 10)
    lcd.print("0");
    lcd.print(seconds);
 
   currentSecond = second();
    
   }
  } 
  
}


bool isTheTime(){
  
  for(int i=1; i<= EEPROM.read(0) ; i++){
    //for(int i=1; i<= 12 ; i++){

    if(EEPROMToDate(i) <= now()){
      
      dateToEEPROM(i, (EEPROMToDate(i)+86400));
      
      
       return true;
    }
     
  }
 
 return false; 
   
}


void getNextFeedTime(){
  
  nextFeedTime = EEPROMToDate(1);
 
 for(int i=2 ; i<= int(EEPROM.read(0)) ; i++){
   //for(int i=2 ; i<= 12 ; i++){

     if(EEPROMToDate(i) < nextFeedTime){
       nextFeedTime = EEPROMToDate(i);
       
    }
     
  }
  
}

void dateToEEPROM(int pos, long value){

      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(pos*4-3, four);
      EEPROM.write(pos*4-2, three);
      EEPROM.write(pos*4-1, two);
      EEPROM.write(pos*4, one);
  
}

long EEPROMToDate(int pos){
 
  //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(pos*4-3);
      long three = EEPROM.read(pos*4-2);
      long two = EEPROM.read(pos*4-1);
      long one = EEPROM.read(pos*4);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF); 
  
}

void ringBell(int times){
  
  for (int i = 0 ; i < times ; i++ ){
    
 digitalWrite(bell, HIGH);
 delay(200);
 digitalWrite(bell, LOW);
 delay(750);
    
  }
  
}



