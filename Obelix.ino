/*

Software del alimentador perruno.

 0 - Bridge RX
 1 - Bridge TX
 2 - SDA i2C Reloj
 3 - SCL i2C Reloj
 4 - Direccion Driver Motor
 5 - PWM Velocidad Driver Motor
 6 - Rele Timbre
 7 - Pulsador 1
 8 - Led Pulsador 1
 9 - Pulsador 2
10 - Led Pulsador 2
11 - RX SoftwareSerial LCD
12 - TX SoftwareSerial LCD
13 - 
A0 - Emisor sensor presencia 1
A1 - Receptor sensor presencia 1
A2 - Emisor sensor presencia 2
A3 - Receptor sensor presencia 2
A4 - Status LED 1 (Atasco)
A5 - Status LED 2 (Vacio)

*/


//Librerias
#include <SoftwareSerial.h>


// Asignaciones de pines
#define direccion 4
#define motor 5
#define timbre 6
#define pulsador1 7
#define pulsador2 9
#define ledpulsador1 8
#define ledpulsador2 10
#define emisor1 A0
#define emisor2 A2
#define sensor1 A1
#define sensor2 A3
#define estatus1 A4
#define estatus2 A5

//Asignaciones de acciones
#define sacar HIGH
#define meter LOW

#define sonar HIGH
#define callar LOW

#define racion 4
#define potencia 200
#define margenopto 200
#define margentiempo 5000

//Declaraciones
bool errAtasco;
bool errVacio;


void setup() {
  
  SoftwareSerial lcd(11, 12); // RX, TX del LCD
  
  pinMode(direccion, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(timbre, OUTPUT);
  pinMode(pulsador1, INPUT_PULLUP);
  pinMode(pulsador2, INPUT_PULLUP);
  pinMode(ledpulsador1, OUTPUT);
  pinMode(ledpulsador2, OUTPUT);
  pinMode(emisor1, OUTPUT);
  pinMode(emisor2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(estatus1, OUTPUT);
  pinMode(estatus2, OUTPUT);
  
  errAtasco = false;
  errVacio = false;

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Comprobar la hora de la comida
  if(!errAtasco && !errVacio){
   //alimentar(raciones);
    
    
  }

}

void alimentar(int raciones){
  
  int valorsensor1 = analogRead(sensor1);
  bool valorbool;
  float tiempoinicio;
  
  if (valorsensor1>margenopto){
    valorbool == true;
  }else{
   valorbool == false; 
  }
 
 for (int i = 0 ; i < raciones ; i++) {
   for(int j = 0 ; j < racion ; j++ ) {
      digitalWrite(direccion, sacar);
      analogWrite(motor, potencia);
      
      tiempoinicio = millis();
           
      if(valorbool){
        while(analogRead(sensor1) > margenopto){
         
          if ((tiempoinicio+margentiempo) > millis()){
             errAtasco = true;
             break;
          }
          delay(20); 
        }
      }else{
         while(analogRead(sensor1) > margenopto){
           if ((tiempoinicio+margentiempo) > millis()){
             errAtasco = true;
             break;
          }
         delay(20); 
        }  
      }
      
      analogWrite(motor, 0);
      
      if(errAtasco) break;
      
   } 
   
   if(errAtasco) break;
 } 
  
}
