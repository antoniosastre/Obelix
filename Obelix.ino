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
//#define xx A2
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

#define entries 2

//Declaraciones

 // RX, TX del LCD
int dispCurMinute = 0;
int dispNextMinute = 0;
long dispNextFeedTime = 0;
int currentSecond = 0;
serLCD lcd(12);

long sc[entries];

long nextFeedTime = 0;


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
lcd.print("   Vers. 0.08   ");
lcd.selectLine(2);
lcd.print("A. Sastre - 2015");

delay(2000);

lcd.clear();

setSyncProvider(RTC.get);

digitalWrite(lred, LOW);


sc[0] = 1421330400;
sc[1] = 1421334400;


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
  
  if(isTheTime()){
    
    giveFood(1);
    getNextFeedTime();
    
  }

  if(digitalRead(butt1)==LOW){
    
    digitalWrite(mdir, give);
    digitalWrite(lbutt1, HIGH);
    analogWrite(mpow, 40);
    
  }else if(digitalRead(butt2)==LOW){
    
    giveFood(1);
    
  }else{
    
   digitalWrite(mdir, give);
   analogWrite(mpow, 0);
   digitalWrite(lbutt1, LOW);
   digitalWrite(lred, LOW);
   digitalWrite(lblue, LOW);
   analogWrite(lgreen, 10);
   digitalWrite(iled, LOW);
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
   
    lcd.setCursor(2,1);
    
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
  
  for(int i=0; i< entries ; i++){

    if(sc[i] <= now()){

       sc[i] = sc[i]+(86400);
       return true;
       
    }
     
  }
 
 return false; 
   
}


void getNextFeedTime(){
  
  nextFeedTime = sc[0];
 
 for(int i=1; i< entries ; i++){

    if(sc[i] < nextFeedTime){

       nextFeedTime = sc[i];
       
    }
     
  }
  
}

