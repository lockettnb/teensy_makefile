//
// 2019/03 arduinon 1.8.8/teensy 1.4.5 example 
//

#include <stdio.h>
#include <string.h>
#include "Arduino.h"

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
  for (i=0; i<20; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(75); 
    digitalWrite(LED_BUILTIN, LOW);   
   delay(55);                      
  }
  
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1500);                      
  digitalWrite(LED_BUILTIN, LOW);    
  delay(2000); digitalWrite(LED_BUILTIN, HIGH);   
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1500);                      
  digitalWrite(LED_BUILTIN, LOW);    
  delay(2000); digitalWrite(LED_BUILTIN, HIGH);   
  
} // while
 
} // main
