/*
   This program is only a demonstration of INPUT_FAST
*/
#define MAX_COUNTER 200000

void setup() {

  Serial.begin(115200);
  
  pinMode(2, INPUT_FAST);  // using the pin 2
  pinMode(3, INPUT);       // using the pin 3
  
  delay(3000); // only to give you time to open the serial debugger terminal
  
  Serial.print("Number of interactions under test:");
  Serial.println(MAX_COUNTER);
    
  unsigned long t0,t;
  unsigned int counter = 0;
  
  
  t0 = micros(); // number microseconds since booted 
  for (counter = 0; counter < MAX_COUNTER; counter++)
  {
       digitalRead(2);   // this is the fast reading !!!!
  }
  
  t=micros()-t0; // delta time
  Serial.print("digitalRead() configured with INPUT_FAST took: ");
  Serial.print(t);
  Serial.println(" microseconds");
  
  t0 = micros(); // reseting to new initial time
  for (counter = 0; counter < MAX_COUNTER; counter++)
  {
     digitalRead(3);   // this is the lazy reading !!!!
  }
 
  t=micros()-t0; // delta time
  Serial.print("digitalRead() configured with INPUT took:");
  Serial.print(t);
  Serial.println(" microseconds");
  
}

void loop() {
}
