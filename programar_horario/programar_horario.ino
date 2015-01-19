#include <EEPROM.h>



void setup() {
  EEPROM.write(0,12);
  
  for(int i = 1 ; i <= 12 ; i++){
    
    dateToEEPROM(i, (1421661600+((i-1)*3600)));
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:

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
