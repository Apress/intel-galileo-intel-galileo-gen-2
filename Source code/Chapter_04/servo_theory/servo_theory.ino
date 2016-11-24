#include <Servo.h> 
 
Servo myservo;  

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 
  
  // in theory 0 degrees
  myservo.writeSeconds(0);
  delay(2000);
  
  // in theory 180 degrees
  myservo.write(2000);
  delay(2000);
   
} 
