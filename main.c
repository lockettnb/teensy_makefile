//
// 2018/03/08 arduinon 1.8.5/teensy 1.4.1 example 
//

#include <stdio.h>
#include <string.h>
#include "Arduino.h"
// #include "HardwareSerial.h"
// #include <avr/io.h>
// #include <avr/interrupt.h>


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

int main(void)
{
  int i;

setup();

// the loop function runs over and over again forever
while (1) {  
  for (i=0; i<50; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(50); 
    digitalWrite(LED_BUILTIN, LOW);   
   delay(155);                      
  }
  
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW);    
  delay(2000); digitalWrite(LED_BUILTIN, HIGH);   
  
} // while
 
} // main
