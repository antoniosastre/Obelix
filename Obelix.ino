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
A4 - Status LED 1
A5 - Status LED 2

*/


// Asignaciones de pines
#define direccion 4
#define velocidad 5
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



//Declaraciones
int errAtasco;
int errVacio;

void setup() {
  
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
