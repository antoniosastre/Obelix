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
A4 - Status LED 1 (Vacio)
A5 - Status LED 2 (Atasco)

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
#define ledvacio A4 //Vacio
#define ledastasco A5 //Atasco

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


////////////////////////
//                    //
//      SETUP()       //
//                    //
////////////////////////

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
  pinMode(ledvacio, OUTPUT);
  pinMode(ledatasco, OUTPUT);
  
  errAtasco = false;
  errVacio = false;

}






////////////////////////
//                    //
//       LOOP()       //
//                    //
////////////////////////




void loop() {
  // put your main code here, to run repeatedly:
  
  //Ver si se quiere alimentar de forma manual
  
  if(digitalRead(pulsador1)==LOW){
    digitalWrite(direccion, sacar);
    analogWrite(motor, potencia);
  }else{
    analogWrite(motor, 0);
  }
  
  //Comprobar el atasco y actualizar la luz.
  if(errAtasco) {
    digitalWrite(ledatasco, HIGH);
  }else{
    digitalWrite(ledatasco, LOW);
  }
  
  
  //Comprobar si vacio y actualizar la luz
  if(isVacio()) {
    digitalWrite(ledvacio, HIGH);
  }else{
    digitalWrite(ledvacio, LOW);
  }
  
  
  //Comprobar la hora de la comida
  if(!errAtasco && !errVacio){
   //alimentar(raciones);
    
    
  }

}




/////////////////////////
//                     //
//      FUNCIONES      //
//                     //
/////////////////////////



////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////
bool isVacio(){
  if(analogRead(sensor2) > margenopto){
   errVacio = true; 
   return true;
  }else{
   errVacio = false; 
   return false;
  }
}
