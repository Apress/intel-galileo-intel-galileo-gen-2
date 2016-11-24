#include <Servo.h> 
 
Servo myservo1;  
Servo myservo2;  

void setup() 
{ 
  myservo1.attach(9);          // attaches the servo to pin 9 at 188 hz
  myservo2.attach(3, true);    // this must work in 48 hz.. All servos will work at 48hz from now
  Serial.begin(115200);
} 
 
 
void loop() 
{ 
  
  
    myservo1.write(180);  
    myservo2.write(0);

    delay(2000);

    myservo1.write(0);  
    myservo2.write(180);
   
    delay(2000);
    
    myservo1.set188hz();; // all servos will work at 188hz
    
} 
