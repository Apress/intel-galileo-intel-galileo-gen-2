#include <Servo.h> 
 
Servo myservo;  

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 
  
  // move to 0 degrees
  myservo.write(0);
  delay(2000);
  
  // move to 180 degrees
  myservo.write(180);
  delay(2000);
   
} 
